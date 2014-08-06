#include "StereoUtil.h"
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

bool hive::isOpenGLStereoAvailable()
{
    // Describes an OpenGL stereo pixel format
    PIXELFORMATDESCRIPTOR pfd = {};
    pfd.nSize = sizeof(pfd);
    pfd.nVersion = 1;
    pfd.dwFlags =
        PFD_DRAW_TO_WINDOW |    // Must be able to draw into a window
        PFD_SUPPORT_OPENGL |    // Must support OpenGL
        PFD_STEREO         |    // Must support OpenGL stereo
        PFD_DOUBLEBUFFER;       // We want a double-buffered format
    pfd.cColorBits = 24;        // Don't care
    pfd.cDepthBits = 16;        // Don't care

    // Get DC for the Desktop
    HDC context = GetDC(0);

    // Attempt to find a matching pixel format
    int pixelFormat = ChoosePixelFormat( context, &pfd );

    // Describe the pixel format
    if ( DescribePixelFormat( context, pixelFormat, sizeof(pfd), &pfd ) == 0 )
        return false;

    // Make sure that PFD_STEREO is set in the flags
    return (pfd.dwFlags & PFD_STEREO) != 0;
}

//-----------------------------------------------------------------------------
