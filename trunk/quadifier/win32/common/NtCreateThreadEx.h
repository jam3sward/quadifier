#ifndef hive_NtCreateThreadEx_h
#define hive_NtCreateThreadEx_h

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

#include <windows.h>
#include <Subauth.h>    // for PUNICODE_STRING

//-----------------------------------------------------------------------------

namespace hive {

//-----------------------------------------------------------------------------

/// Windows Vista kernel sources: probably correct
typedef struct _CLIENT_ID {
    PVOID   UniqueProcess;
    PVOID   UniqueThread;
} CLIENT_ID, *PCLIENT_ID;

//-----------------------------------------------------------------------------

/// Undocumented: based on guesswork
typedef struct _THREAD_INFO {
    ULONG   Flags;          ///< Flags
    ULONG   BufferSize;     ///< Size of buffer in bytes
    PVOID   lpBuffer;       ///< Pointer to buffer
    ULONG   Unknown;        ///< Typically zero
} THREAD_INFO, *PTHREAD_INFO;

//-----------------------------------------------------------------------------

/// Undocumented: based on guesswork
typedef struct _CREATE_THREAD_INFO {
    ULONG       Length;     ///< Size of structure in bytes
    THREAD_INFO Client;     ///< Unknown
    THREAD_INFO TEB;        ///< User mode stack context?
} CREATE_THREAD_INFO;

//-----------------------------------------------------------------------------

/// Could include from Ntdef.h (this avoids having to install the WDK)
typedef struct _OBJECT_ATTRIBUTES {
    ULONG           Length;
    HANDLE          RootDirectory;
    PUNICODE_STRING ObjectName;
    ULONG           Attributes;
    PVOID           SecurityDescriptor;
    PVOID           SecurityQualityOfService;
}  OBJECT_ATTRIBUTES, *POBJECT_ATTRIBUTES;

//-----------------------------------------------------------------------------

/// Undocumented NtCreateThreadEx function
NTSTATUS NtCreateThreadEx(
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
    OUT CREATE_THREAD_INFO      *ThreadInfo     // guesswork
);

//-----------------------------------------------------------------------------

/// Simplified version of NtCreateThreadEx
HANDLE SimpleNtCreateThreadEx(
    HANDLE hProcess,
    LPTHREAD_START_ROUTINE lpStartAddress,
    LPVOID lpParameter
);

//-----------------------------------------------------------------------------

} // namespace hive

//-----------------------------------------------------------------------------

#endif//hive_NtCreateThreadEx_h