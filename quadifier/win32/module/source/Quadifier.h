#ifndef hive_Quadifier_h
#define hive_Quadifier_h

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

#include <d3d9.h>
#include <GL/gl.h>
#include <GL/glext.h>
#include <GL/wglext.h>
#include <array>
#include <set>
#include "Event.h"
#include "CriticalSection.h"
#include "Extensions.h"
#include "GLWindow.h"

//-----------------------------------------------------------------------------

/// Quadifier is the main implementation of the Direct3D to OpenGL renderer
class Quadifier {
public:
    /// Constructor
    Quadifier(
        IDirect3DDevice9 *device,
        IDirect3D9 *direct3D
    );

    /// Destructor
    virtual ~Quadifier();

    /// Called when D3D device is created
    void onCreateDX();

    /// Called immediately before D3D Clear
    void onPreClearDX(
        DWORD Count,
        CONST D3DRECT *pRects,
        DWORD Flags,
        D3DCOLOR Color,
        float Z,
        DWORD Stencil
    );

    /// Called immediately after D3D Clear
    void onPostClearDX();

    /// Called immediately before D3D Present
    void onPrePresentDX(
        CONST RECT *pSourceRect,
        CONST RECT *pDestRect,
        HWND hDestWindowOverride,
        CONST RGNDATA *pDirtyRegion
    );

    /// Called immediately after D3D Present
    void onPostPresentDX();

private:
    /// Create D3D resources (render targets)
    void createResources();

    /// Start OpenGL rendering thread
    void startRenderThread();

    /// Called when OpenGL window is created
    bool onCreate();

    /// Called when OpenGL window is destroyed
    void onDestroy();

    /// Called when OpenGL window is painted
    void onPaint();
    
    /// Called when OpenGL window is resized
    void onResize( UINT type, int w, int h );

    /// Called to perform idle processing
    void onIdle();

    /// Request a redraw
    void redraw();

public:
    /// The WIN32 WindowProc for the OpenGL window
    LRESULT CALLBACK windowProc(
        HWND hWnd,      // handle to window
        UINT uMsg,      // message identifier
        WPARAM wParam,  // first message parameter
        LPARAM lParam   // second message parameter
    );

    /// OpenGL rendering thread function
    static unsigned __stdcall threadFunc( void *context );

private:

    /// Return the wall clock time in seconds
    double getTime() const;

    /**
     * Send the last rendered frame from DX to GL and swap the render
     * targets ready for next frame.
     */
    void sendFrame( GLuint drawBuffer );

    /**
     * Returns true if the current render target has ever been presented
     * (which we use to detect render targets that are actually displayed,
     * rather than the various off screen render targets).
     */
    bool isPresentedRenderTarget() const;

private:
    IDirect3DDevice9    *m_device;      ///< The Direct3D device
    IDirect3D9          *m_direct3D;    ///< The Direct3D interface
    LPDIRECT3DSURFACE9   m_backBuffer;  ///< The back buffer for rendering

    unsigned m_framesGL;    ///< OpenGL frame count
    unsigned m_framesDX;    ///< Direct3D frame count

    unsigned m_samplesDX;   ///< Direct3D multisamples (or 0)
    unsigned m_samplesGL;   ///< OpenGL multisamples (or 0)

    unsigned m_drawBuffer;  ///< buffer to draw to
    unsigned m_readBuffer;  ///< buffer to read from

    unsigned m_width;       ///< display width in pixels
    unsigned m_height;      ///< display height in pixels

    bool m_initialised;     ///< has initialisation completed?

    /// Set of all render targets that have been presented
    std::set< unsigned > m_presentedTargets;

    /// Stores all the details of an individual render target
    struct Target {
        LPDIRECT3DSURFACE9  surface;        ///< Direct3D surface
        HANDLE              object;         ///< Handle of interop object
        GLuint              texture;        ///< OpenGL texture
        GLuint              renderBuffer;   ///< OpenGL renderbuffer
        GLuint              frameBuffer;    ///< OpenGL framebuffer
        GLuint              drawBuffer;     ///< OpenGL draw buffer identifier

        /// Default constructor
        Target() :
            surface(0),
            object(0),
            texture(0),
            renderBuffer(0),
            frameBuffer(0),
            drawBuffer(GL_BACK)
        {
        }

        /// Clear the contents (free stored data)
        void clear() {
            if ( surface != 0 ) {
                surface->Release();
                surface = 0;
            }
        }
    };

    std::tr1::array<Target,3> m_target; ///< DX/GL targets for rendering

    bool     m_verbose;             ///< Verbose logging

    bool     m_stereoMode;          ///< Stereo mode enable/disable
    bool     m_stereoAvailable;     ///< Is quad-buffer stereo available?
    unsigned m_clearCount;          ///< Number of clears per frame
    unsigned m_clearCountPersist;   ///< Persistent number of clears

    double   m_firstFrameTimeGL;    ///< time-stamp of first GL frame
    double   m_lastFrameTimeGL;     ///< time-stamp of last GL frame

    GLuint   m_quadListGL;          ///< GL display list for textured quad

    uintptr_t m_thread;             ///< Handle of the rendering thread

    HWND m_sourceWindow;            ///< Window handle of source (Direct3D)

    HANDLE m_interopGLDX;           ///< Handle for the OpenGL/DX interop

    Event m_frameDone;              ///< Signals when frame is rendered out

    Extensions glx;                 ///< Stores the OpenGL extension functions

    GLWindow m_window;              ///< The OpenGL output window
};

//-----------------------------------------------------------------------------

#endif//hive_Quadifier_h
