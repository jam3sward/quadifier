#ifndef Settings_h
#define Settings_h

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

#include <string>

//-----------------------------------------------------------------------------

struct Settings {
    bool passThrough;       ///< Enable "pass through" mode
    bool forceDirect3D9Ex;  ///< Force Direct3D9 applications to use Direct3D9Ex
    bool useTexture;        ///< Use textures (true) or renderbuffers (false)
    bool preventModeChange; ///< Prevent application from changing display mode
    bool matchOriginalMSAA; ///< Should GL use same number of samples as DX?

    /// Returns the current settings
    static const Settings & get();

    /// Load settings from file
    bool load( const std::string & fileName );

private:
    Settings();
};

//-----------------------------------------------------------------------------

#endif//Settings_h
