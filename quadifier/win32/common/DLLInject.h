#ifndef hive_DLLInject_h
#define hive_DLLInject_h

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
#include <string>

//-----------------------------------------------------------------------------

namespace hive {

/**
 * Injects a DLL into the specified process, given the process ID (PID) and the
 * filename of the DLL to inject. Returns true for success, false otherwise.
 */
bool injectDLL( DWORD processId, const std::string & DLLName );

/**
 * Creates a process and injects a DLL into it. Returns true for success, false
 * otherwise.
 */
bool createProcessWithDLL(
    const std::string & applicationName,
    const std::string & DLLName,
    const std::string & commandLine = "",
    const std::string & currentDirectory = ""
);

} // namespace hive

//-----------------------------------------------------------------------------

#endif//hive_DLLInject_h