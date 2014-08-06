#include <GL/gl.h>
#include <GL/glx.h>
#include <dlfcn.h>
#include <vector>
#include "Log.h"

using namespace std;

//-----------------------------------------------------------------------------
//
// Copyright (C) 2013-14 James Ward, University of Hull
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

// Quadifier for Linux
// J.Ward 16/12/13
// Usage:
//   LD_PRELOAD=$PWD/quadifer.so <YourExecutableHere>

//-----------------------------------------------------------------------------

bool g_stereoDetect = false;    ///< have we detected incoming stereo frames?
unsigned g_clearsPerEye = 0;    ///< number of glClear calls per eye
unsigned g_clearCount   = 0;    ///< used to count number of glClear calls

//-----------------------------------------------------------------------------

/// Class used to perform one-time initialisation at load time
class Init {
public:
    Init() {
        Log::get()
            .setLevel( Log::Detailed )
            .open( "quadifier.log" );
    }
} g_init;

//-----------------------------------------------------------------------------

/// define function type for glClear
typedef void FN_glClear( GLbitfield mask );

/// define function type for glXSwapBuffers
typedef void FN_glXSwapBuffers( Display *dpy, GLXDrawable drawable );

/// define function type for glXChooseFBConfig
typedef GLXFBConfig* FN_glXChooseFBConfig(
    Display *dpy, int screen,
    const int *attribList, int *nitems
);

//-----------------------------------------------------------------------------

/// hook calls to glClear()
void glClear( GLbitfield mask )
{
    if ( Log::verbose() )
        Log::stream() << "glClear" << endl;

    // obtain pointer to original function
    static FN_glClear *original = reinterpret_cast<FN_glClear*>(
        dlsym( RTLD_NEXT, "glClear" )
    );
    if ( original == 0 ) {
        // failed to obtain original function pointer
        if ( Log::error() )
            Log::stream() << "Failed to hook glClear" << endl;
        return;
    }

    // if stereo is enabled, select the appropriate left/right buffer,
    // otherwise select the back buffer
    if ( !g_stereoDetect )
        glDrawBuffer( GL_BACK );
    else if ( g_clearCount < g_clearsPerEye )
        glDrawBuffer( GL_BACK_LEFT );
    else
        glDrawBuffer( GL_BACK_RIGHT );

    // call the original function
    original( mask );

    // count the number of glClear calls
    ++g_clearCount;
}

//-----------------------------------------------------------------------------

/// hook calls to glXSwapBuffers
void glXSwapBuffers( Display *dpy, GLXDrawable drawable )
{
    if ( Log::verbose() )
        Log::stream() << "glXSwapBuffers" << endl;

    // obtain pointer to original function
    FN_glXSwapBuffers *original = reinterpret_cast<FN_glXSwapBuffers*>(
        dlsym( RTLD_NEXT, "glXSwapBuffers" )
    );
    if ( original == 0 ) {
        // failed to obtain original function pointer
        if ( Log::error() )
            Log::stream() << "Failed to hook glXSwapBuffers" << endl;
        return;
    }

    // call the original function
    original( dpy, drawable );

    // was stereo detected previously?
    bool wasStereo = g_stereoDetect;

    // detected stereo if there is more than one glClear per frame, and the
    // number of glClear per frame is exactly divisible by two
    g_stereoDetect = (g_clearCount > 1) && ((g_clearCount % 2) == 0);
    
    // if stereo is enabled, then g_clearsPerEye indicates how many glClear
    // calls are expected per eye, otherwise zero
    if ( g_stereoDetect )
        g_clearsPerEye = g_clearCount / 2;
    else
        g_clearsPerEye = 0;

    if ( Log::verbose() ) {
        // log the number of glClears per frame
        if ( g_clearCount > 1 )
            Log::stream() << g_clearCount << " clears per frame" << endl;
    }

    if ( Log::detailed() ) {
        // detect when stereo is enabled/disabled
        if ( g_stereoDetect && !wasStereo ) {
            Log::stream()
                << "stereo enabled: "
                << g_clearCount   << " sub-frames detected "
                << "(" << g_clearsPerEye << " per eye)" << endl;
        } else if ( wasStereo && !g_stereoDetect )
            Log::stream() << "stereo disabled" << endl;
    }

    // reset counter for next time
    g_clearCount=0;
}

//-----------------------------------------------------------------------------

/// hook calls to glXChooseFBConfig
GLXFBConfig *glXChooseFBConfig(
    Display *dpy, int screen,
    const int *attribList, int *nitems
) {
    if ( Log::verbose() )
        Log::stream() << "glXChooseFBConfig" << endl;

    // obtain pointer to original function
    FN_glXChooseFBConfig *original = reinterpret_cast<FN_glXChooseFBConfig*>(
        dlsym( RTLD_NEXT, "glXChooseFBConfig" )
    );
    if ( original == 0 ) {
        // failed to obtain original function pointer
        if ( Log::error() )
            Log::stream() << "Failed to hook glXChooseFBConfig" << endl;
        return 0;
    }

    // buffer to store new attributes
    std::vector<GLint> newAttrib;

    // copy all the attributes, ensuring that any existing GLX_STEREO
    // attribute is set to true
    bool stereoAlreadySet = false;
    unsigned numAttrib = 0;
    for (int n=0; attribList[n] != None; n+=2) {
        // copy the attribute name
        newAttrib.push_back( attribList[n] );

        // count number of attributes
        ++numAttrib;

        // is this the stereo attribute?
        if ( attribList[n] == GLX_STEREO ) {
            // yes: ensure the value is true
            newAttrib.push_back( GL_TRUE );
            stereoAlreadySet = true;
        } else {
            // no: copy the value verbatim
            newAttrib.push_back( attribList[n+1] );
        }
    }

    if ( Log::detailed() ) {
        Log::stream()
            << "glXChooseFBConfig: "
            << numAttrib << " attributes passed" << endl;
    }

    // if the stereo attribute wasn't present (most likely), then append it
    // and set it to true
    if ( !stereoAlreadySet ) {
        newAttrib.push_back( GLX_STEREO );
        newAttrib.push_back( GL_TRUE );
        if ( Log::detailed() )
            Log::stream() << "added GLX_STEREO attribute" << endl;
    } else {
        if ( Log::detailed() )
            Log::stream() << "set existing GLX_STEREO attribute to GL_TRUE" << endl;
    }

    // add the terminating 'None' value
    newAttrib.push_back( None );

    // call the real function
    return original( dpy, screen, &newAttrib[0], nitems );
}

//-----------------------------------------------------------------------------

