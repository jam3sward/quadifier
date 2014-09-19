#ifndef hive_GLWindow_h
#define hive_GLWindow_h

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
#include <vector>
#include <map>

//-----------------------------------------------------------------------------

/// This class represents an OpenGL window under WIN32
class GLWindow {
public:
    /// Default constructor
    GLWindow();

    /// Destructor
    virtual ~GLWindow();

    /// Used to define WGL attributes for window creation
    typedef std::map<int,int> Attributes;

    /// Empty attributes (default value)
    static Attributes emptyAttributes;

    /// Create window
    bool create(
        DWORD exStyle,
        LPCTSTR windowName,
        DWORD style,
        int x,
        int y,
        int width,
        int height,
        HWND parent,
        HMENU menu,
        WNDPROC windowProc,
        LPVOID lpParam,
        const Attributes & userAttributes = emptyAttributes
    );

    /// Destroy window
    void destroy();

    /// Show the window
    void show( int showCommand = SW_SHOW );

    /// Swap buffers
    void swapBuffers();
    
    /// Invalidate client area
    void invalidate();

    /// Update window
    void update();

    /// Returns the window handle
    HWND getHWND() const;

    /// Returns the device context
    HDC getHDC() const;

    /// Returns the OpenGL resource context
    HGLRC getHGLRC() const;

    /// Returns the OpenGL pixel format
    int getPixelFormat() const;

    /// Queries number of multisamples from OpenGL
    unsigned getSamples() const;

private:
    /// Copy construction is unsupported
    GLWindow( const GLWindow & );

    /// Assignment is unsupported
    GLWindow & operator = ( const GLWindow & );

private:
    HWND     m_hwnd;        ///< window handle
    HDC      m_hdc;         ///< device context
    HGLRC    m_hglrc;       ///< OpenGL resource context
    int      m_pixelFormat; ///< OpenGL pixel format
};

//-----------------------------------------------------------------------------

#endif//hive_GLWindow_h
