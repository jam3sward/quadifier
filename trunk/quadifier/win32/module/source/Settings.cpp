#include "Settings.h"
#include <Windows.h>

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

#include <fstream>
#include <algorithm>
using namespace std;

#include "Log.h"
using namespace hive;

//-----------------------------------------------------------------------------

const Settings & Settings::get()
{
    static Settings settings;
    return settings;
}

//-----------------------------------------------------------------------------

bool Settings::load( const std::string & fileName )
{
    // open the file
    ifstream input( fileName.c_str() );
    if ( !input ) return false;

    struct {
        // convert string to boolean
        bool readBool( std::string text ) {
            // convert to lower case
            std::transform( text.begin(), text.end(), text.begin(), ::tolower );

            // accept sensible values for true (anything else is false)
            return ( text == "yes" ) || ( text == "true" ) || ( text == "1" );
        }
    } local;

    do {
        // read key
        string key;
        input >> ws >> key;
        if ( !input ) break;

        // read value
        string value;
        input >> ws >> value;
        if ( !input ) {
            // missing values return an error
            return false;
        }

        Log::print("Settings: ") << key << " = " << value << endl;

        // store key/value
        if ( key == "passThrough" )
            passThrough = local.readBool( value );
        else if ( key == "useTexture" )
            useTexture = local.readBool( value );
        else if ( key == "preventModeChange" )
            preventModeChange = local.readBool( value );
        else {
            // unrecognised keys are currently ignored
        }

    } while (true);

    // close the file
    input.close();

    return true;
}

//-----------------------------------------------------------------------------

Settings::Settings() :
    passThrough( false ),
    forceDirect3D9Ex( false ),
    useTexture( false ),
    preventModeChange( true )
{
    // initialise OS version info structure
    OSVERSIONINFO info = {};
    info.dwOSVersionInfoSize = sizeof(info);

    // are we on Windows Vista or higher?
    bool vistaUp = GetVersionEx(&info) && (info.dwMajorVersion >= 6);

    // force use of Direct3D9Ex on Vista upwards
    forceDirect3D9Ex = vistaUp;

    // attempt to load settings from file
    static const std::string fileName( "quadifier.ini" );
    if ( !load( fileName ) )
        Log::print() << "Failed to load settings from [" << fileName << "]\n";
}

//-----------------------------------------------------------------------------
