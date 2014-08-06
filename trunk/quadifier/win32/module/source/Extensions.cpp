#include "Extensions.h"

//-----------------------------------------------------------------------------

Extensions::Extensions() :
    wglDXSetResourceShareHandleNV(0),
    wglDXOpenDeviceNV(0),
    wglDXCloseDeviceNV(0),
    wglDXRegisterObjectNV(0),
    wglDXUnregisterObjectNV(0),
    wglDXObjectAccessNV(0),
    wglDXLockObjectsNV(0),
    wglDXUnlockObjectsNV(0),
    glGenFramebuffers(0),
    glBindFramebuffer(0),
    glFramebufferTexture2D(0),
    glBindRenderbuffer(0),
    glGenRenderbuffers(0),
    glDeleteRenderbuffers(0),
    glFramebufferRenderbuffer(0),
    glBlitFramebuffer(0),
    glCheckFramebufferStatus(0),
    glGetRenderbufferParameteriv(0)
{
}

//-----------------------------------------------------------------------------

bool Extensions::load()
{
    wglDXSetResourceShareHandleNV =
        reinterpret_cast<PFNWGLDXSETRESOURCESHAREHANDLENVPROC>
            ( wglGetProcAddress( "wglDXSetResourceShareHandleNV" ) );

    bool success = ( wglDXSetResourceShareHandleNV != 0 );

    wglDXOpenDeviceNV = reinterpret_cast<PFNWGLDXOPENDEVICENVPROC>
        ( wglGetProcAddress( "wglDXOpenDeviceNV" ) );

    success = success && ( wglDXOpenDeviceNV != 0 );

    wglDXCloseDeviceNV = reinterpret_cast<PFNWGLDXCLOSEDEVICENVPROC>
        ( wglGetProcAddress( "wglDXCloseDeviceNV" ) );

    success = success && ( wglDXCloseDeviceNV != 0 );

    wglDXRegisterObjectNV = reinterpret_cast<PFNWGLDXREGISTEROBJECTNVPROC>
        ( wglGetProcAddress( "wglDXRegisterObjectNV" ) );

    success = success && ( wglDXRegisterObjectNV != 0 );

    wglDXUnregisterObjectNV = reinterpret_cast<PFNWGLDXUNREGISTEROBJECTNVPROC>
        ( wglGetProcAddress( "wglDXUnregisterObjectNV" ) );

    success = success && ( wglDXUnregisterObjectNV != 0 );

    wglDXObjectAccessNV = reinterpret_cast<PFNWGLDXOBJECTACCESSNVPROC>
        ( wglGetProcAddress( "wglDXObjectAccessNV" ) );

    success = success && ( wglDXObjectAccessNV != 0 );

    wglDXLockObjectsNV = reinterpret_cast<PFNWGLDXLOCKOBJECTSNVPROC>
        ( wglGetProcAddress( "wglDXLockObjectsNV" ) );

    success = success && ( wglDXLockObjectsNV != 0 );

    wglDXUnlockObjectsNV =
        reinterpret_cast<PFNWGLDXUNLOCKOBJECTSNVPROC>
            ( wglGetProcAddress( "wglDXUnlockObjectsNV" ) );

    success = success && ( wglDXUnlockObjectsNV != 0 );

    glGenFramebuffers =
        reinterpret_cast<PFNGLGENFRAMEBUFFERSPROC>
            ( wglGetProcAddress( "glGenFramebuffers" ) );

    success = success && ( glGenFramebuffers != 0 );

    glBindFramebuffer =
        reinterpret_cast<PFNGLBINDFRAMEBUFFERPROC>
            ( wglGetProcAddress( "glBindFramebuffer" ) );

    success = success && ( glBindFramebuffer != 0 );

    glFramebufferTexture2D =
        reinterpret_cast<PFNGLFRAMEBUFFERTEXTURE2DPROC>
            ( wglGetProcAddress( "glFramebufferTexture2D" ) );

    success = success && ( glBindFramebuffer != 0 );

    glBindRenderbuffer =
        reinterpret_cast<PFNGLBINDRENDERBUFFERPROC>
            ( wglGetProcAddress( "glBindRenderbuffer" ) );

    success = success && ( glBindRenderbuffer != 0 );

    glGenRenderbuffers =
        reinterpret_cast<PFNGLGENRENDERBUFFERSPROC>
            ( wglGetProcAddress( "glGenRenderbuffers" ) );

    success = success && ( glGenRenderbuffers != 0 );

    glRenderbufferStorage =
        reinterpret_cast<PFNGLRENDERBUFFERSTORAGEPROC>
            ( wglGetProcAddress( "glRenderbufferStorage" ) );

    success = success && ( glRenderbufferStorage != 0 );

    glDeleteRenderbuffers =
        reinterpret_cast<PFNGLDELETERENDERBUFFERSPROC>
            ( wglGetProcAddress( "glDeleteRenderbuffers" ) );

    success = success && ( glDeleteRenderbuffers != 0 );

    glFramebufferRenderbuffer =
        reinterpret_cast<PFNGLFRAMEBUFFERRENDERBUFFERPROC>
            ( wglGetProcAddress( "glFramebufferRenderbuffer" ) );

    success = success && ( glFramebufferRenderbuffer != 0 );

    glBlitFramebuffer =
        reinterpret_cast<PFNGLBLITFRAMEBUFFERPROC>
            ( wglGetProcAddress( "glBlitFramebuffer" ) );

    success = success && ( glBlitFramebuffer != 0 );

    glCheckFramebufferStatus =
        reinterpret_cast<PFNGLCHECKFRAMEBUFFERSTATUSPROC>
            ( wglGetProcAddress( "glCheckFramebufferStatus" ) );

    success = success && ( glCheckFramebufferStatus != 0 );

    glGetRenderbufferParameteriv =
        reinterpret_cast<PFNGLGETRENDERBUFFERPARAMETERIVPROC>
            ( wglGetProcAddress( "glGetRenderbufferParameteriv" ) );

    success = success && ( glGetRenderbufferParameteriv != 0 );

    return success;
}//load

//-----------------------------------------------------------------------------
