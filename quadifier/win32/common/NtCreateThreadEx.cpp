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

using namespace hive;

//-----------------------------------------------------------------------------

/// Function pointer to the undocumented NtCreateThreadEx function
typedef NTSTATUS (WINAPI *LPFN_NtCreateThreadEx)
(
    OUT PHANDLE                 hThread,
    IN  ACCESS_MASK             DesiredAccess,
    IN  POBJECT_ATTRIBUTES      ObjectAttributes,
    IN  HANDLE                  ProcessHandle,
    IN  LPTHREAD_START_ROUTINE  lpStartAddress,
    IN  LPVOID                  lpParameter,
    IN  BOOL                    CreateSuspended,
    IN  DWORD                   StackZeroBits,
    IN  DWORD                   SizeOfStackCommit,
    IN  DWORD                   SizeOfstackReserve,
    OUT CREATE_THREAD_INFO      *ThreadInfo         // guesswork
);

//-----------------------------------------------------------------------------

NTSTATUS hive::NtCreateThreadEx(
    OUT PHANDLE                 hThread,
    IN  ACCESS_MASK             DesiredAccess,
    IN  POBJECT_ATTRIBUTES      ObjectAttributes,
    IN  HANDLE                  ProcessHandle,
    IN  LPTHREAD_START_ROUTINE  lpStartAddress,
    IN  LPVOID                  lpParameter,
    IN  BOOL                    CreateSuspended,
    IN  DWORD                   StackZeroBits,
    IN  DWORD                   SizeOfStackCommit,
    IN  DWORD                   SizeOfStackReserve,
    OUT CREATE_THREAD_INFO      *ThreadInfo         // guesswork
) {
    /// This structure manages a reference to NTDLL::NtCreateThreadEx
    static struct NTDLL {
        NTDLL() {
            // load library at entry
            module = LoadLibrary( "ntdll.dll" );

            if ( module != 0 ) {
                // obtain function pointer to NtCreateThreadEx
                NtCreateThreadEx = reinterpret_cast<LPFN_NtCreateThreadEx>(
                    GetProcAddress( module, "NtCreateThreadEx" )
                );
            } else {
                // failed
                NtCreateThreadEx = 0;
            }
        }

        ~NTDLL() {
            // free library at exit
            if ( module != 0 ) FreeLibrary( module );
        }

        HMODULE module;
        LPFN_NtCreateThreadEx NtCreateThreadEx;
    } ntdll;

    // ensure we have a valid function pointer
    if ( ntdll.NtCreateThreadEx == 0 ) return 0;

    // call the actual function
    return ntdll.NtCreateThreadEx(
        hThread,
        DesiredAccess,
        ObjectAttributes,
        ProcessHandle,
        lpStartAddress,
        lpParameter,
        CreateSuspended,
        StackZeroBits,
        SizeOfStackCommit,
        SizeOfStackReserve,
        ThreadInfo
    );
}

//-----------------------------------------------------------------------------

HANDLE hive::SimpleNtCreateThreadEx(
    HANDLE hProcess,
    LPTHREAD_START_ROUTINE lpStartAddress,
    LPVOID lpParameter
) {
    CLIENT_ID clientId = {};
    CLIENT_ID teb = {};

    // The values passed in this structure are based on values observed
    // during debugging (because NtCreateThreadEx is undocumented).
    CREATE_THREAD_INFO info = {};
    info.Length = sizeof(info);

    info.Client.Flags       = 0x10003;
    info.Client.BufferSize  = sizeof(clientId);
    info.Client.lpBuffer    = &clientId;

    info.TEB.Flags          = 0x10004;
    info.TEB.BufferSize     = sizeof(clientId);
    info.TEB.lpBuffer       = &teb;

    // Receives the thread handle
    HANDLE hThread = 0;

    // Create the thread
    ::NtCreateThreadEx(
        &hThread,       // returns thread handle
        GENERIC_ALL,    // access rights
        0,
        hProcess,       // handle of process
        lpStartAddress, // thread start address
        lpParameter,    // thread user defined parameter
        FALSE,          // start immediately (don't create suspended)
        0,
        0,
        0,
        &info
    );

    // Return thread handle (or zero)
    return hThread;
}

//-----------------------------------------------------------------------------
