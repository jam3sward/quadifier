#include "GLWindow.h"
#include <GL/GL.h>
#include <GL/glext.h>
#include <GL/wglext.h>
#include <tchar.h>

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

GLWindow::Attributes GLWindow::emptyAttributes;

//-----------------------------------------------------------------------------

GLWindow::GLWindow() :
    m_hwnd(0),
    m_hdc(0),
    m_hglrc(0),
    m_pixelFormat(0)
{
}

//-----------------------------------------------------------------------------

GLWindow::~GLWindow()
{
    // destroy the window
    destroy();
}

//-----------------------------------------------------------------------------

bool GLWindow::create(
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
    const Attributes & userAttributes
) {
    // destroy existing window (if any)
    destroy();

    // window class name
    static const _TCHAR *className = _T("GLCLASS");

    // define window class
    WNDCLASS wc = {};
    wc.style         = CS_OWNDC | CS_HREDRAW | CS_VREDRAW;
    wc.lpfnWndProc   = windowProc;
    wc.hInstance     = GetModuleHandle( 0 );
    wc.hIcon         = LoadIcon  ( 0, IDI_APPLICATION );
    wc.hCursor       = LoadCursor( 0, IDC_ARROW );
    wc.hbrBackground = 0;
    wc.lpszMenuName  = _T(""); 
    wc.lpszClassName = className;

    HWND window = 0;
    HDC context = 0;
    HGLRC glcontext = 0;
    int pixelFormat = 0;
    int multisamples = 0;

    do {
        // register window class
        RegisterClass( &wc );

        // create a temporary window
        window = CreateWindowEx(
            0,                      // extended style
            className,              // class name
            _T(""),                 // window title
            WS_OVERLAPPEDWINDOW |   // window style
            WS_CLIPCHILDREN     |
            WS_CLIPSIBLINGS,
            0, 0,                   // position
            8, 8,                   // size
            0,                      // parent
            0,                      // menu
            wc.hInstance,           // application instance handle
            0                       // window creation data
        );
        if ( window == 0 ) break;

        // get device context for window
        context = GetDC( window );
        if ( context == 0 ) break;

        // setup pixel format descriptor
        PIXELFORMATDESCRIPTOR pfd = {};
        pfd.nSize        = sizeof(pfd);
        pfd.nVersion     = 1;
        pfd.dwFlags      = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL;
        pfd.cColorBits   = 32;
        pfd.cDepthBits   = 24;
        pfd.cStencilBits = 8;
        pfd.iLayerType   = PFD_MAIN_PLANE;

        // choose the pixel format
        pixelFormat = ChoosePixelFormat( context, &pfd );
        if ( pixelFormat == 0 ) break;

        // attempt to set the pixel format
        if ( SetPixelFormat( context, pixelFormat, &pfd ) != TRUE ) break;

        // create OpenGL context
        glcontext = wglCreateContext( context );
        if ( glcontext == 0 ) break;

        // attempt to make OpenGL context current
        if ( wglMakeCurrent( context, glcontext ) != TRUE ) break;

        // obtain function pointer to wglChoosePixelFormatARB
        PFNWGLCHOOSEPIXELFORMATARBPROC wglChoosePixelFormatARB =
            reinterpret_cast<PFNWGLCHOOSEPIXELFORMATARBPROC>(
                wglGetProcAddress( "wglChoosePixelFormatARB" ) );
        if ( wglChoosePixelFormatARB == 0 ) break;

        // define our default pixel format attributes
        Attributes attributes;
        attributes[WGL_DRAW_TO_WINDOW_ARB] = GL_TRUE;
        attributes[WGL_SUPPORT_OPENGL_ARB] = GL_TRUE;
        attributes[WGL_DOUBLE_BUFFER_ARB ] = GL_TRUE;
        attributes[WGL_ACCELERATION_ARB  ] = WGL_FULL_ACCELERATION_ARB;
        attributes[WGL_PIXEL_TYPE_ARB    ] = WGL_TYPE_RGBA_ARB;
        attributes[WGL_COLOR_BITS_ARB    ] = 32;
        attributes[WGL_DEPTH_BITS_ARB    ] = 24;
        attributes[WGL_STENCIL_BITS_ARB  ] = 8;

        // insert the user defined attributes (which can override defaults)
        attributes.insert( userAttributes.begin(), userAttributes.end() );

        // flatten the map to an integer vector and terminate it with 0
        vector<int> data;
        for ( Attributes::iterator it=attributes.begin(); it!=attributes.end(); ++it) {
            data.push_back( it->first );
            data.push_back( it->second );
        }
        data.push_back( 0 );
       
        pixelFormat = 0;
        UINT numFormats = 0;

        // choose closest pixel format to the attributes above
        if ( wglChoosePixelFormatARB(
            context, &data[0], 0, 1, &pixelFormat, &numFormats
        ) != TRUE ) {
            pixelFormat = 0;
            break;
        }
    } while (0,0);

    // delete temporary OpenGL context
    if ( glcontext != 0 ) {
        wglMakeCurrent( context, 0 );
        wglDeleteContext( glcontext );
        glcontext = 0;
    }

    // destroy temporary window
    if ( window != 0 ) {
        DestroyWindow( window );
        window = 0;
    }

    // if we failed to find a pixel format, exit now
    if ( pixelFormat == 0 ) return false;

    // now we try to create the window properly
    do {
        // create a window
        window = CreateWindowEx(
            exStyle,        // extended style
            className,      // class name
            windowName,     // window title
            style,          // style
            x, y,           // position
            width, height,  // size
            parent,         // parent
            menu,           // menu
            wc.hInstance,   // application instance handle
            lpParam         // window creation data
        );
        if ( window == 0 ) break;

        // get device context for window
        context = GetDC( window );
        if ( context == 0 ) break;

        // attempt to describe the pixel format
        PIXELFORMATDESCRIPTOR pfd = {};
        pfd.nSize = sizeof(pfd);
        pfd.nVersion = 1;
        if ( DescribePixelFormat( context, pixelFormat, sizeof(pfd), &pfd ) == 0 )
            break;

        // attempt to set the pixel format
        if ( SetPixelFormat( context, pixelFormat, &pfd ) != TRUE ) break;

        // create OpenGL context
        glcontext = wglCreateContext( context );
        if ( glcontext == 0 ) break;

        // attempt to make OpenGL context current
        if ( wglMakeCurrent( context, glcontext ) != TRUE ) break;

        // enable multisample if required
        if ( multisamples > 0 ) glEnable( GL_MULTISAMPLE_ARB );

        // at this point we have succeeded, so we can populate the member
        // variables and return
        m_hwnd  = window;
        m_hdc   = context;
        m_hglrc = glcontext;
        m_pixelFormat = pixelFormat;

        // success
        return true;
    } while (0,0);

    // something failed, and we need to clean up

    // delete context
    if ( glcontext != 0 ) {
        wglMakeCurrent( 0, 0 );
        wglDeleteContext( glcontext );
        context = 0;
    }

    // destroy window
    if ( window != 0 ) {
        DestroyWindow( window );
        window = 0;
    }

    // failure
    return false;
}

//-----------------------------------------------------------------------------

void GLWindow::destroy()
{
    if ( m_hglrc != 0 ) {
        wglMakeCurrent( 0, 0 );
        wglDeleteContext( m_hglrc );
        m_hglrc = 0;
    }

    if ( m_hwnd != 0 ) {
        DestroyWindow( m_hwnd );
        m_hwnd = 0;
    }

    m_hdc = 0;
    m_pixelFormat = 0;
}

//-----------------------------------------------------------------------------

void GLWindow::show( int showCommand )
{
    if ( m_hwnd != 0 ) ShowWindow( m_hwnd, showCommand );
}

//-----------------------------------------------------------------------------

void GLWindow::swapBuffers()
{
    if ( m_hdc != 0 ) SwapBuffers( m_hdc );
}

//-----------------------------------------------------------------------------

void GLWindow::invalidate()
{
    if ( m_hwnd != 0 ) InvalidateRect( m_hwnd, 0, FALSE );
}

//-----------------------------------------------------------------------------

void GLWindow::update()
{
    if ( m_hwnd != 0 ) UpdateWindow( m_hwnd );
}

//-----------------------------------------------------------------------------

HWND GLWindow::getHWND() const
{
    return m_hwnd;
}

//-----------------------------------------------------------------------------

HDC GLWindow::getHDC() const
{
    return m_hdc;
}

//-----------------------------------------------------------------------------

HGLRC GLWindow::getHGLRC() const
{
    return m_hglrc;
}

//-----------------------------------------------------------------------------

unsigned GLWindow::getSamples() const
{
    GLint samples = 0;
    glGetIntegerv( GL_SAMPLES, &samples );
    if ( samples < 0 ) samples = 0;
    return static_cast<unsigned>( samples );
}

//-----------------------------------------------------------------------------

int GLWindow::getPixelFormat() const
{
    return m_pixelFormat;
}

//-----------------------------------------------------------------------------
