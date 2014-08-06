#include <iostream>
#include "DLLInject.h"

using namespace std;

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

int main ( int argc, char **argv )
{
    // the name of our DLL to be injected
    static const string moduleName( "module.dll" );

    // have we got an application filename?
    if ( argc > 1 ) {
        // name of the application executable
        string applicationName( argv[1] );

        // build command line string
        string commandLine;
        for (int i=2; i<argc; ++i) {
            if (i > 2) commandLine += " ";
            commandLine += string(argv[i]);
        }

        // attempt to launch the application and inject the DLL
        if ( !hive::createProcessWithDLL(
            applicationName,    // name of executable
            moduleName,         // name of DLL
            commandLine         // command line options
        ) )
            cerr << "error: failed to start " << applicationName << endl;
    } else {
        // display usage instructions
        cerr << "usage: launcher <executable> [options]\n";
    }

    return 0;
}

//-----------------------------------------------------------------------------
