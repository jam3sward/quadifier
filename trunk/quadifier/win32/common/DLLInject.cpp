#include "DLLInject.h"
#include <assert.h>
#include <memory>
#include <vector>
#include <iostream>
#include <Tlhelp32.h>
#include <windows.h>
#include <VersionHelpers.h>
#include "NtCreateThreadEx.h"

//-----------------------------------------------------------------------------
//
// Copyright (C) 2012-14 James Ward, University of Hull
//
// This software is provided 'as-is', without any express or implied
// warranty. In no event will the authors be held liable for any damages
// arising from the use of this software.
//
// Permission is granted to anyone to use this software for any purpose,
// including commercial applications, and to alter it and redistribute it
// freely, subject to the following restrictions:
//
//    1. The origin of this software must not be misrepresented; you must not
//    claim that you wrote the original software.
//
//    2. If you use this software in a product, an acknowledgment in the
//    product documentation is required.
//
//    3. Altered source versions must be plainly marked as such, and must not
//    be misrepresented as being the original software.
//
//    4. This notice may not be removed or altered from any source
//    distribution.
//
//-----------------------------------------------------------------------------

using namespace std;

//-----------------------------------------------------------------------------

namespace {

#ifndef PCLIENT_ID
    /// define PCLIENT_ID if not already defined
    typedef PVOID PCLIENT_ID;
#endif

/// prototype for the undocumented RtlCreateUserThread function from ntdll.dll
typedef NTSYSAPI NTSTATUS NTAPI FNRtlCreateUserThread(
  IN HANDLE               processHandle,
  IN PSECURITY_DESCRIPTOR securityDescriptor OPTIONAL,
  IN BOOLEAN              createSuspended,
  IN ULONG                stackZeroBits,
  IN OUT PULONG           stackReserved,
  IN OUT PULONG           stackCommit,
  IN PVOID                startAddress,
  IN PVOID                startParameter OPTIONAL,
  OUT PHANDLE             threadHandle,
  OUT PCLIENT_ID          clientID
);

/// prototype for the undocumented RtlExitUserThread function from ntdll.dll
typedef NTSYSAPI VOID NTAPI FNRtlExitUserThread(
    IN NTSTATUS	status
);

/// define function pointer for LoadLibraryA
typedef HMODULE (WINAPI *PFNLoadLibraryA)( LPCTSTR );

/// define function pointer for ExitThread
typedef VOID (WINAPI *PFNExitThread)( DWORD );

/// define function pointer for RtlExitUserThread
typedef FNRtlExitUserThread *PFNRtlExitUserThread;

/// define function pointer for GetLastError
typedef DWORD (WINAPI *PFNGetLastError)( void );

/// Data structure to be passed to our remote thread
struct Data {
    PFNLoadLibraryA         LoadLibrary;
    PFNExitThread           ExitThread;
    PFNRtlExitUserThread    RtlExitUserThread;
    PFNGetLastError         GetLastError;
    void                   *threadExitFunc;         ///< Thread exit function
    char                    pathName[_MAX_PATH];    ///< DLL path name
} data;

#ifdef _M_X64
// The 64-bit loader code is linked from an external assembler file,
// because inline assembler isn't supported for 64-bit
extern "C" { void loaderCode(void*);}
extern "C" { void loaderCodeEnd(void);}
#endif//_M_X64

#ifdef _M_IX86
/**
 * This 32-bit code is run on a thread created inside the target process.
 * It loads the specified DLL then exits the thread.
 */
static __declspec(naked) DWORD WINAPI loaderCode( LPVOID /*param*/ ) {
    __asm {
        mov eax, dword ptr [ESP+4]
        push ebx
        mov ebx, eax
        add eax, Data::pathName;
        push eax
        call [ebx + Data::LoadLibrary]
        test eax,eax	
        jz failure
        xor eax,eax
        jmp done
        failure:
        call [ebx + Data::GetLastError]
        done:
        push eax
        call [ebx + Data::threadExitFunc]       // call thread exit function
        pop ebx
        ret 4
    }
} static __declspec(naked) void loaderCodeEnd() { __asm ret 4 }
#endif//_M_IX86

} // namespace

//-----------------------------------------------------------------------------

/// Enable a requested privilege for the specified process. Returns true for
/// success or false in case of failure.
bool enablePrivilege( HANDLE process, LPCSTR privilege )
{
    // check input parameters
    if ( (process == 0) || (privilege == 0) ) return false;

    // open the process token
    HANDLE token = 0;
    if ( OpenProcessToken( process, TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, &token) == FALSE )
        return false;

    // look up the locally unique identifier for the given privilege name
    LUID luid = {};
    if ( LookupPrivilegeValue( 0, privilege, &luid ) == FALSE ) {
        CloseHandle( token );
        return false;
    }
    
    // this structure defines the privilege (luid) to be enabled
    TOKEN_PRIVILEGES newState = {};
    newState.PrivilegeCount            = 1;
    newState.Privileges[0].Luid        = luid;
    newState.Privileges[0].Attributes  = SE_PRIVILEGE_ENABLED;
     
    // enable the privilege
    bool result = ( AdjustTokenPrivileges(
        token,              // token handle
        FALSE,              // disable all priviliges?
        &newState,          // new privilege state
        sizeof(newState),   // size of new state
        0,                  // previous state
        0                   // return length
    ) == TRUE );

    // close the process token
    CloseHandle( token );

    // return true in case of success, false otherwise
    return result;
}//enablePrivilege

//-----------------------------------------------------------------------------

bool hive::injectDLL( DWORD processId, const std::string & pathName )
{
    HANDLE process = 0;
    HANDLE thread  = 0;
    LPVOID memory  = 0;
    unsigned memorySize = 0;

    bool result = false;

    // zero fill the data structure
    memset( &data, 0, sizeof(data) );

    // load kernel32
    HMODULE kernel32 = LoadLibrary( "kernel32.dll" );
    if ( kernel32 == 0 ) return false;

    // load ntdll
    HMODULE ntdll = LoadLibrary( "ntdll.dll" );
    if ( ntdll == 0 ) return false;

    do {
        // load the DLL in the current process without executing, so that
        // we can obtain the full path name (guarantees that we use the
        // same DLL search algorithm as windows)
        HMODULE dll = LoadLibraryEx( 
            pathName.c_str(), 0, DONT_RESOLVE_DLL_REFERENCES
        );

        // get the full path to the DLL module, which we will use later
        // when injecting the DLL into the target process
        TCHAR fullPath[_MAX_PATH] = {};
        if ( (dll == 0) ||
            (GetModuleFileName( dll, fullPath, sizeof(fullPath) ) == 0)
        ) {
            // failure: fall back to the given path
            strncpy_s(
                fullPath, sizeof(fullPath),
                pathName.c_str(), _TRUNCATE
            );
        }

        // free the DLL
        if ( dll != 0) FreeLibrary( dll );

        // attempt to enable debug privilege
        // we ignore the outcome, as the code below does not usually need this
        enablePrivilege( GetCurrentProcess(), SE_DEBUG_NAME );

        // open the target process so that we can inject the DLL
        process = OpenProcess(
            PROCESS_CREATE_THREAD   |
            PROCESS_VM_OPERATION    |
            PROCESS_VM_READ         |
            PROCESS_VM_WRITE        |
            PROCESS_QUERY_INFORMATION,
            FALSE, processId
        );
        if ( process == 0 ) break;

        // obtain function pointer to LoadLibraryA and store in data
        data.LoadLibrary = reinterpret_cast<PFNLoadLibraryA>(
            GetProcAddress( kernel32, "LoadLibraryA" ) );
        if ( data.LoadLibrary == 0 ) break;

        // obtain function pointer to GetLastError and store in data
        data.GetLastError = reinterpret_cast<PFNGetLastError>(
            GetProcAddress( kernel32, "GetLastError" ) );
        if ( data.GetLastError == 0 ) break;

        // obtain function pointer to ExitThread and store in data
        data.ExitThread = reinterpret_cast<PFNExitThread>(
            GetProcAddress( kernel32, "ExitThread" ) );
        if ( data.ExitThread == 0 ) break;

        // obtain function pointer to RtlExitUserThread and store in data
        data.RtlExitUserThread = reinterpret_cast<PFNRtlExitUserThread>(
            GetProcAddress( ntdll, "RtlExitUserThread" ) );
        if ( data.RtlExitUserThread == 0 ) break;

        // copy the DLL path name into the data structure
        strncpy_s(
            data.pathName, sizeof(data.pathName),
            fullPath, _TRUNCATE
        );

        // obtain a pointer to the undocumented RtlCreateUserThread function
        FNRtlCreateUserThread *RtlCreateUserThread =
            reinterpret_cast<FNRtlCreateUserThread*>(
                GetProcAddress( ntdll, "RtlCreateUserThread" )
            );
        if ( RtlCreateUserThread == 0 ) break;

        // convert pointer types so we can do arithmetic with them
        char *codeBeginPtr = reinterpret_cast<char*>(loaderCode);
        char *codeEndPtr   = reinterpret_cast<char*>(loaderCodeEnd);

        // verify assumption that codeEnd is assembled after codeBegin
        assert( codeEndPtr > codeBeginPtr );
        if ( codeEndPtr <= codeBeginPtr ) break;

        // calculate size of inline assembler code
        size_t codeSize = codeEndPtr - codeBeginPtr;

        // size of data
        unsigned dataSize = sizeof(data);

        // total memory required
        size_t memorySize = codeSize + dataSize;

        // allocate memory in target process
        memory = VirtualAllocEx(
            process,                    // handle of the target process
            0,                          // preferred address
            memorySize,                 // size of allocation in bytes
            MEM_COMMIT | MEM_RESERVE,   // allocation type
            PAGE_EXECUTE_READWRITE      // protection
        );
        if ( memory == 0 ) break;

        // copy the thread code into the target process
        SIZE_T numWritten = 0;
        LPVOID codeAddress = memory;
        WriteProcessMemory(
            process, codeAddress,
            loaderCode, codeSize,
            &numWritten
        );
        if ( numWritten != codeSize ) break;

        // calculate address where data should be written in the remote process
        // (it's located immediately after the code)
        // the data consists of function pointers and the DLL path name
        LPVOID dataAddress = reinterpret_cast<char*>(memory) + codeSize;

        /// helper class used to write data into the remote process
        struct Writer {
            /// constructor initialises the process, address and data pointers
            Writer(
                HANDLE process, LPVOID baseAddress, LPCVOID buffer, SIZE_T size
            ) :
                m_process( process ),
                m_baseAddress( baseAddress ),
                m_buffer( buffer ),
                m_size( size )
            {
            }

            /// writes the current data structure into process memory,
            /// returning true for success, false in case of failure
            bool writeProcessMemory() {
                SIZE_T numWritten = 0;
                WriteProcessMemory(
                    m_process, m_baseAddress,
                    m_buffer, m_size,
                    &numWritten
                );
                return ( numWritten == m_size );
            }
        
        private:
            HANDLE  m_process;      ///< process handle
            LPVOID  m_baseAddress;  ///< base address to write
            LPCVOID m_buffer;       ///< data to be written
            SIZE_T  m_size;         ///< size of data

        } local(                    // initialisation
            process, dataAddress,
            &data, dataSize
        );

        // initialise OS version info structure
        OSVERSIONINFO info = {};
        info.dwOSVersionInfoSize = sizeof(info);

        // are we on Windows Vista or above?
        bool vistaUp = IsWindowsVistaOrGreater();

        // We will try three different methods to create our thread inside the
        // remote process:
        // - CreateRemoteThread
        // - NtCreateThreadEx (undocumented function)
        // - RtlCreateUserThread (undocumented function)

        // CreateRemoteThread
        // -install the appropriate thread exit function
        data.threadExitFunc = data.ExitThread;
        if ( !local.writeProcessMemory() ) break;
        // -create remote thread and execute code inside target process
        thread = CreateRemoteThread(
            process, 0, 0,
            reinterpret_cast<LPTHREAD_START_ROUTINE>(codeAddress),
            reinterpret_cast<LPVOID>(dataAddress),
            0, 0
        );

        if ( ( thread == 0 ) && vistaUp ) {
            // NtCreateThreadEx
            // -install the appropriate thread exit function
            data.threadExitFunc = data.RtlExitUserThread;
            if ( !local.writeProcessMemory() ) break;
            // -create remote thread and execute code inside target process
            thread = SimpleNtCreateThreadEx(
                process,
                (LPTHREAD_START_ROUTINE)codeAddress,
                dataAddress
            );
        }

        if ( ( thread == 0 ) && vistaUp ) {
            // RtlCreateUserThread
            // -install the appropriate thread exit function
            data.threadExitFunc = data.RtlExitUserThread;
            if ( !local.writeProcessMemory() ) break;

            // -create remote thread and execute code inside target process,
            NTSTATUS threadStatus = RtlCreateUserThread(
                process, NULL, 0, 0, 0, 0,
                codeAddress,        // address of code
                dataAddress,        // address of data
                &thread,
                NULL
            );

            // exit in case of failure
            if ( threadStatus != 0 ) break;
        }

        // wait for thread to terminate
        WaitForSingleObject( thread, INFINITE );

        // get the thread exit code
        DWORD exitCode = 0;
        GetExitCodeThread( thread, &exitCode );

        // success depends on thread exit code
        result = (exitCode == ERROR_SUCCESS);
    } while (0,0);

    // close the thread handle
    if ( thread != 0 ) CloseHandle( thread );

    // free the memory used in the remote process
    if ( memory != 0 ) VirtualFreeEx( process, memory, memorySize, MEM_RELEASE );

    // close the remote process handle
    if ( process != 0 ) CloseHandle( process );

    // free ntdll
    if ( ntdll != 0 ) FreeLibrary( ntdll );

    // free kernel32
    if ( kernel32 != 0 ) FreeLibrary( kernel32 );

    return result;
}//injectDLL

//-----------------------------------------------------------------------------

bool hive::createProcessWithDLL(
    const std::string & applicationName,
    const std::string & DLLName,
    const std::string & commandLine,
    const std::string & currentDirectory
) {
    STARTUPINFO startupInfo = { sizeof(startupInfo) };
    PROCESS_INFORMATION processInfo = {};

    // copy commandLine to non const null terminated string
    std::vector<char> command( commandLine.begin(), commandLine.end() );
    command.push_back( 0 );

    // create the process with the main thread suspended
    if ( !CreateProcess(
        applicationName.c_str(),
        (command.size() > 1) ? &command[0] : 0,		// non const
        0,
        0,
        FALSE,
        CREATE_SUSPENDED,
        0,
        !currentDirectory.empty() ? currentDirectory.c_str() : 0,
        &startupInfo,
        &processInfo
    ) ) {
        cerr << "failed to create process" << endl;
        return false;
    }

    // attempt to inject the DLL
    bool injected = false;
    int  attempts = 0;
    for (; (attempts<2) && !injected; ++attempts)
        injected = injectDLL( processInfo.dwProcessId, DLLName );

    if ( !injected )
        cerr << "error: failed to inject" << endl;

    if ( attempts > 1 )
        cerr << attempts << " injection attempts" << endl;

    // resume the main thread
    if ( ResumeThread( processInfo.hThread ) == static_cast<DWORD>(-1) ) {
        TerminateThread( processInfo.hThread, 0 );
        return false;
    }

    // wait for it to start up (as near as we can tell)
    Sleep(1000);
    WaitForInputIdle( processInfo.hProcess, 1000 );

    // close process and thread handles
    CloseHandle( processInfo.hProcess );
    CloseHandle( processInfo.hThread );

    return true;
}//createProcessWithDLL

//-----------------------------------------------------------------------------
