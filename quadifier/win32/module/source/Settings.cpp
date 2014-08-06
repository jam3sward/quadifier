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

const Settings & Settings::get()
{
    static Settings settings;
    return settings;
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
}

//-----------------------------------------------------------------------------
