#ifndef hive_Extensions_h
#define hive_Extensions_h

//-----------------------------------------------------------------------------

#include <windows.h>
#include <GL/gl.h>
#include <GL/glext.h>
#include <GL/wglext.h>

//-----------------------------------------------------------------------------

struct Extensions {
    PFNWGLDXSETRESOURCESHAREHANDLENVPROC    wglDXSetResourceShareHandleNV;
    PFNWGLDXOPENDEVICENVPROC                wglDXOpenDeviceNV;
    PFNWGLDXCLOSEDEVICENVPROC               wglDXCloseDeviceNV;
    PFNWGLDXREGISTEROBJECTNVPROC            wglDXRegisterObjectNV;
    PFNWGLDXUNREGISTEROBJECTNVPROC          wglDXUnregisterObjectNV;
    PFNWGLDXOBJECTACCESSNVPROC              wglDXObjectAccessNV;
    PFNWGLDXLOCKOBJECTSNVPROC               wglDXLockObjectsNV;
    PFNWGLDXUNLOCKOBJECTSNVPROC             wglDXUnlockObjectsNV;
    PFNGLGENFRAMEBUFFERSPROC                glGenFramebuffers;
    PFNGLBINDFRAMEBUFFERPROC                glBindFramebuffer;
    PFNGLFRAMEBUFFERTEXTURE2DPROC           glFramebufferTexture2D;
    PFNGLBINDRENDERBUFFERPROC               glBindRenderbuffer;
    PFNGLGENRENDERBUFFERSPROC               glGenRenderbuffers;
    PFNGLRENDERBUFFERSTORAGEPROC            glRenderbufferStorage;
    PFNGLDELETERENDERBUFFERSPROC            glDeleteRenderbuffers;
    PFNGLFRAMEBUFFERRENDERBUFFERPROC        glFramebufferRenderbuffer;
    PFNGLBLITFRAMEBUFFERPROC                glBlitFramebuffer;
    PFNGLCHECKFRAMEBUFFERSTATUSPROC         glCheckFramebufferStatus;
    PFNGLGETRENDERBUFFERPARAMETERIVPROC     glGetRenderbufferParameteriv;

    Extensions();

    bool load();
};

//-----------------------------------------------------------------------------

#endif//hive_Extensions_h
