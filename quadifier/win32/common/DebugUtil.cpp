#include "DebugUtil.h"
#include <sstream>
#include <atlconv.h>
using namespace std;

#include <d3d9.h>
#include <D3D11.h>
#include <GL/GL.h>
#include <GL/glext.h>

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

std::string formatErrorMessage( DWORD error )
{
    USES_CONVERSION;

    WCHAR *lpMsgBuf = 0;

    // convert error code to wide string
    if ( !FormatMessage(
        FORMAT_MESSAGE_ALLOCATE_BUFFER | 
        FORMAT_MESSAGE_FROM_SYSTEM     |
        FORMAT_MESSAGE_IGNORE_INSERTS,
        0,
        error,
        MAKELANGID( LANG_NEUTRAL, SUBLANG_DEFAULT ),
        (LPWSTR)&lpMsgBuf,
        0, 0
    ) ) {
        // format message failed: output error code instead
        stringstream ss;
        ss << "Unknown error 0x" << hex << error << endl;
        return ss.str();
    }

    // convert to STL string
    string text( W2A((LPCWSTR)lpMsgBuf) );

    // free storage
    LocalFree( lpMsgBuf );

    // return message
    return text;
}

//-----------------------------------------------------------------------------

std::string GUIDtoString( const GUID & guid )
{
    USES_CONVERSION;

    // convert GUID to wide string in hex format
    OLECHAR *wide;
    StringFromCLSID( guid, &wide );

    // convert wide string to string
    string text( W2A(wide) );

    // free memory
    ::CoTaskMemFree( wide );

    return text;
}

//-----------------------------------------------------------------------------

std::string GUIDtoObjectName( const GUID & guid )
{
    // convert GUID to registry format string
    string guidString( GUIDtoString(guid) );

    // construct full registry path
    string fullPath( string("Interface\\") + guidString );

    // buffer to receive name
    char buffer[256];
    DWORD bufferSize = sizeof(buffer);
    if ( RegQueryValueExA(
        HKEY_CLASSES_ROOT,
        fullPath.c_str(),
        0,
        0,
        reinterpret_cast<LPBYTE>(buffer),
        &bufferSize
    ) == ERROR_SUCCESS) {
        return buffer;
    } else {
        if ( guid == __uuidof(IDXGIDevice) )
            return "IDXGIDevice";
        else if ( guid == __uuidof(IDXGIAdapter) )
            return "IDXGIAdapter";
        else if ( guid == __uuidof(IDXGIFactory) )
            return "IDXGIFactory";

        // fall back to using GUID string
        return guidString;
    }
}

//-----------------------------------------------------------------------------

std::string D3DUSAGEtoString( DWORD usage )
{
    // string stream used to collect individual names
    stringstream ss;

    // table mapping bit mask to name
    static const struct {
        DWORD usage;
        const char *name;
    } table[] = {
        { D3DUSAGE_AUTOGENMIPMAP, "D3DUSAGE_AUTOGENMIPMAP" },
        { D3DUSAGE_DEPTHSTENCIL, "D3DUSAGE_DEPTHSTENCIL" },
        { D3DUSAGE_DMAP, "D3DUSAGE_DMAP" },
        { D3DUSAGE_DONOTCLIP, "D3DUSAGE_DONOTCLIP" },
        { D3DUSAGE_DYNAMIC, "D3DUSAGE_DYNAMIC" },
        { D3DUSAGE_NONSECURE, "D3DUSAGE_NONSECURE" },
        { D3DUSAGE_NPATCHES, "D3DUSAGE_NPATCHES" },
        { D3DUSAGE_POINTS, "D3DUSAGE_POINTS" },
        { D3DUSAGE_RENDERTARGET, "D3DUSAGE_RENDERTARGET" },
        { D3DUSAGE_RTPATCHES, "D3DUSAGE_RTPATCHES" },
        { D3DUSAGE_SOFTWAREPROCESSING, "D3DUSAGE_SOFTWAREPROCESSING" },
        { D3DUSAGE_TEXTAPI, "D3DUSAGE_TEXTAPI" },
        { D3DUSAGE_WRITEONLY, "D3DUSAGE_WRITEONLY" },
        { D3DUSAGE_RESTRICTED_CONTENT, "D3DUSAGE_RESTRICTED_CONTENT" },
        { D3DUSAGE_RESTRICT_SHARED_RESOURCE, "D3DUSAGE_RESTRICT_SHARED_RESOURCE" },
        { D3DUSAGE_RESTRICT_SHARED_RESOURCE_DRIVER, "D3DUSAGE_RESTRICT_SHARED_RESOURCE_DRIVER" },
        {0, 0}
    };

    // for each bit in table
    unsigned count = 0;
    for ( unsigned i=0; table[i].name !=0; ++i ) {
        // if the bit is set, append the name to the string stream
        if ( (usage & table[i].usage) != 0 ) {
            if ( count > 0 ) ss << " | ";
            ss << table[i].name;
            ++count;
        }
    }

    // return contents of string stream
    return ss.str();
}

//-----------------------------------------------------------------------------

std::string D3DPOOLtoString( D3DPOOL pool )
{
    switch ( pool ) {
        case D3DPOOL_DEFAULT		:return "D3DPOOL_DEFAULT";
        case D3DPOOL_MANAGED		:return "D3DPOOL_MANAGED";
        case D3DPOOL_SYSTEMMEM	:return "D3DPOOL_SYSTEMMEM";
        case D3DPOOL_SCRATCH		:return "D3DPOOL_SCRATCH";
        case D3DPOOL_FORCE_DWORD:return "D3DPOOL_FORCE_DWORD";
        default					:return "";
    }
}

//-----------------------------------------------------------------------------

std::string D3DFORMATtoString( D3DFORMAT format )
{
    switch ( format ) {
    case D3DFMT_UNKNOWN         :return "D3DFMT_UNKNOWN";
    case D3DFMT_R8G8B8          :return "D3DFMT_R8G8B8";
    case D3DFMT_A8R8G8B8        :return "D3DFMT_A8R8G8B8";
    case D3DFMT_X8R8G8B8        :return "D3DFMT_X8R8G8B8";
    case D3DFMT_R5G6B5          :return "D3DFMT_R5G6B5";
    case D3DFMT_X1R5G5B5        :return "D3DFMT_X1R5G5B5";
    case D3DFMT_A1R5G5B5        :return "D3DFMT_A1R5G5B5";
    case D3DFMT_A4R4G4B4        :return "D3DFMT_A4R4G4B4";
    case D3DFMT_R3G3B2          :return "D3DFMT_R3G3B2";
    case D3DFMT_A8              :return "D3DFMT_A8";
    case D3DFMT_A8R3G3B2        :return "D3DFMT_A8R3G3B2";
    case D3DFMT_X4R4G4B4        :return "D3DFMT_X4R4G4B4";
    case D3DFMT_A2B10G10R10     :return "D3DFMT_A2B10G10R10";
    case D3DFMT_A8B8G8R8        :return "D3DFMT_A8B8G8R8";
    case D3DFMT_X8B8G8R8        :return "D3DFMT_X8B8G8R8";
    case D3DFMT_G16R16          :return "D3DFMT_G16R16";
    case D3DFMT_A2R10G10B10     :return "D3DFMT_A2R10G10B10";
    case D3DFMT_A16B16G16R16    :return "D3DFMT_A16B16G16R16";
    case D3DFMT_A8P8            :return "D3DFMT_A8P8";
    case D3DFMT_P8              :return "D3DFMT_P8";
    case D3DFMT_L8              :return "D3DFMT_L8";
    case D3DFMT_A8L8            :return "D3DFMT_A8L8";
    case D3DFMT_A4L4            :return "D3DFMT_A4L4";
    case D3DFMT_V8U8            :return "D3DFMT_V8U8";
    case D3DFMT_L6V5U5          :return "D3DFMT_L6V5U5";
    case D3DFMT_X8L8V8U8        :return "D3DFMT_X8L8V8U8";
    case D3DFMT_Q8W8V8U8        :return "D3DFMT_Q8W8V8U8";
    case D3DFMT_V16U16          :return "D3DFMT_V16U16";
    case D3DFMT_A2W10V10U10     :return "D3DFMT_A2W10V10U10";
    case D3DFMT_UYVY            :return "D3DFMT_UYVY";
    case D3DFMT_R8G8_B8G8       :return "D3DFMT_R8G8_B8G8";
    case D3DFMT_YUY2            :return "D3DFMT_YUY2";
    case D3DFMT_G8R8_G8B8       :return "D3DFMT_G8R8_G8B8";
    case D3DFMT_DXT1            :return "D3DFMT_DXT1";
    case D3DFMT_DXT2            :return "D3DFMT_DXT2";
    case D3DFMT_DXT3            :return "D3DFMT_DXT3";
    case D3DFMT_DXT4            :return "D3DFMT_DXT4";
    case D3DFMT_DXT5            :return "D3DFMT_DXT5";
    case D3DFMT_D16_LOCKABLE    :return "D3DFMT_D16_LOCKABLE";
    case D3DFMT_D32             :return "D3DFMT_D32";
    case D3DFMT_D15S1           :return "D3DFMT_D15S1";
    case D3DFMT_D24S8           :return "D3DFMT_D24S8";
    case D3DFMT_D24X8           :return "D3DFMT_D24X8";
    case D3DFMT_D24X4S4         :return "D3DFMT_D24X4S4";
    case D3DFMT_D16             :return "D3DFMT_D16";
    case D3DFMT_D32F_LOCKABLE   :return "D3DFMT_D32F_LOCKABLE";
    case D3DFMT_D24FS8          :return "D3DFMT_D24FS8";
    case D3DFMT_D32_LOCKABLE    :return "D3DFMT_D32_LOCKABLE";
    case D3DFMT_S8_LOCKABLE     :return "D3DFMT_S8_LOCKABLE";
    case D3DFMT_L16             :return "D3DFMT_L16";
    case D3DFMT_VERTEXDATA      :return "D3DFMT_VERTEXDATA";
    case D3DFMT_INDEX16         :return "D3DFMT_INDEX16";
    case D3DFMT_INDEX32         :return "D3DFMT_INDEX32";
    case D3DFMT_Q16W16V16U16    :return "D3DFMT_Q16W16V16U16";
    case D3DFMT_MULTI2_ARGB8    :return "D3DFMT_MULTI2_ARGB8";
    case D3DFMT_R16F            :return "D3DFMT_R16F";
    case D3DFMT_G16R16F         :return "D3DFMT_G16R16F";
    case D3DFMT_A16B16G16R16F   :return "D3DFMT_A16B16G16R16F";
    case D3DFMT_R32F            :return "D3DFMT_R32F";
    case D3DFMT_G32R32F         :return "D3DFMT_G32R32F";
    case D3DFMT_A32B32G32R32F   :return "D3DFMT_A32B32G32R32F";
    case D3DFMT_CxV8U8          :return "D3DFMT_CxV8U8";
    case D3DFMT_A1              :return "D3DFMT_A1";
    case D3DFMT_A2B10G10R10_XR_BIAS :return "D3DFMT_A2B10G10R10_XR_BIAS";
    case D3DFMT_BINARYBUFFER    :return "D3DFMT_BINARYBUFFER";
    case D3DFMT_FORCE_DWORD     :return "D3DFMT_FORCE_DWORD";
    default                     :return "";
    }
}

//-----------------------------------------------------------------------------			

std::string D3DMULTISAMPLE_TYPEtoString( D3DMULTISAMPLE_TYPE type )
{
    switch ( type ) {
    case D3DMULTISAMPLE_NONE        :return "D3DMULTISAMPLE_NONE";
    case D3DMULTISAMPLE_NONMASKABLE :return "D3DMULTISAMPLE_NONMASKABLE";
    case D3DMULTISAMPLE_2_SAMPLES   :return "D3DMULTISAMPLE_2_SAMPLES";
    case D3DMULTISAMPLE_3_SAMPLES   :return "D3DMULTISAMPLE_3_SAMPLES";
    case D3DMULTISAMPLE_4_SAMPLES   :return "D3DMULTISAMPLE_4_SAMPLES";
    case D3DMULTISAMPLE_5_SAMPLES   :return "D3DMULTISAMPLE_5_SAMPLES";
    case D3DMULTISAMPLE_6_SAMPLES   :return "D3DMULTISAMPLE_6_SAMPLES";
    case D3DMULTISAMPLE_7_SAMPLES   :return "D3DMULTISAMPLE_7_SAMPLES";
    case D3DMULTISAMPLE_8_SAMPLES   :return "D3DMULTISAMPLE_8_SAMPLES";
    case D3DMULTISAMPLE_9_SAMPLES   :return "D3DMULTISAMPLE_9_SAMPLES";
    case D3DMULTISAMPLE_10_SAMPLES  :return "D3DMULTISAMPLE_10_SAMPLES";
    case D3DMULTISAMPLE_11_SAMPLES  :return "D3DMULTISAMPLE_11_SAMPLES";
    case D3DMULTISAMPLE_12_SAMPLES  :return "D3DMULTISAMPLE_12_SAMPLES";
    case D3DMULTISAMPLE_13_SAMPLES  :return "D3DMULTISAMPLE_13_SAMPLES";
    case D3DMULTISAMPLE_14_SAMPLES  :return "D3DMULTISAMPLE_14_SAMPLES";
    case D3DMULTISAMPLE_15_SAMPLES  :return "D3DMULTISAMPLE_15_SAMPLES";
    case D3DMULTISAMPLE_16_SAMPLES  :return "D3DMULTISAMPLE_16_SAMPLES";
    case D3DMULTISAMPLE_FORCE_DWORD :return "D3DMULTISAMPLE_FORCE_DWORD";
    default                         :return "";
    }
}

//-----------------------------------------------------------------------------			

std::string D3DFVFtoString( DWORD fvf )
{
    if ( fvf == 0 ) return "non-FVF";

    stringstream ss;

    // table mapping bit mask to name
    static const struct {
        DWORD usage;
        const char *name;
    } table[] = {
        { D3DFVF_RESERVED0,"D3DFVF_RESERVED0" },
        { D3DFVF_POSITION_MASK,"D3DFVF_POSITION_MASK" },
        { D3DFVF_XYZ,"D3DFVF_XYZ" },
        { D3DFVF_XYZRHW,"D3DFVF_XYZRHW" },
        { D3DFVF_XYZB1,"D3DFVF_XYZB1" },
        { D3DFVF_XYZB2,"D3DFVF_XYZB2" },
        { D3DFVF_XYZB3,"D3DFVF_XYZB3" },
        { D3DFVF_XYZB4,"D3DFVF_XYZB4" },
        { D3DFVF_XYZB5,"D3DFVF_XYZB5" },
        { D3DFVF_XYZW,"D3DFVF_XYZW" },
        { D3DFVF_NORMAL,"D3DFVF_NORMAL" },
        { D3DFVF_PSIZE,"D3DFVF_PSIZE" },
        { D3DFVF_DIFFUSE,"D3DFVF_DIFFUSE" },
        { D3DFVF_SPECULAR,"D3DFVF_SPECULAR" },
        { D3DFVF_TEXCOUNT_MASK,"D3DFVF_TEXCOUNT_MASK" },
        { D3DFVF_TEXCOUNT_SHIFT,"D3DFVF_TEXCOUNT_SHIFT" },
        { D3DFVF_TEX0,"D3DFVF_TEX0" },
        { D3DFVF_TEX1,"D3DFVF_TEX1" },
        { D3DFVF_TEX2,"D3DFVF_TEX2" },
        { D3DFVF_TEX3,"D3DFVF_TEX3" },
        { D3DFVF_TEX4,"D3DFVF_TEX4" },
        { D3DFVF_TEX5,"D3DFVF_TEX5" },
        { D3DFVF_TEX6,"D3DFVF_TEX6" },
        { D3DFVF_TEX7,"D3DFVF_TEX7" },
        { D3DFVF_TEX8,"D3DFVF_TEX8" },
        { D3DFVF_LASTBETA_UBYTE4,"D3DFVF_LASTBETA_UBYTE4" },
        { D3DFVF_LASTBETA_D3DCOLOR,"D3DFVF_LASTBETA_D3DCOLOR" },
        { D3DFVF_RESERVED2,"D3DFVF_RESERVED2" },
        { 0, 0 }
    };

    // for each bit in table
    unsigned count = 0;
    for ( unsigned i=0; table[i].name !=0; ++i ) {
        // if the bit is set, append the name to the string stream
        if ( (fvf & table[i].usage) != 0 ) {
            if ( count > 0 ) ss << " | ";
            ss << table[i].name;
            ++count;
        }
    }

    // return contents of string stream
    return ss.str();
}

//-----------------------------------------------------------------------------			

std::string getGLErrorString() {
    GLenum error = glGetError();

    switch ( error ) {
    case GL_NO_ERROR            : return "GL_NO_ERROR";
    case GL_INVALID_ENUM        : return "GL_INVALID_ENUM";
    case GL_INVALID_VALUE       : return "GL_INVALID_VALUE";
    case GL_INVALID_OPERATION   : return "GL_INVALID_OPERATION";
    case GL_STACK_OVERFLOW      : return "GL_STACK_OVERFLOW";
    case GL_STACK_UNDERFLOW     : return "GL_STACK_UNDERFLOW";
    case GL_OUT_OF_MEMORY       : return "GL_OUT_OF_MEMORY";
    case GL_INVALID_FRAMEBUFFER_OPERATION:
        return "GL_INVALID_FRAMEBUFFER_OPERATION";
    };

    stringstream text;
    text << "[GL error " << hex << error << "]";
    return text.str();
}

//-----------------------------------------------------------------------------

std::string GLFRAMEBUFFERSTATUStoString( GLenum status ) {
    switch ( status ) {
    case GL_FRAMEBUFFER_COMPLETE:
        return "GL_FRAMEBUFFER_COMPLETE";
    case GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT:
        return "GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT";
    //case GL_FRAMEBUFFER_INCOMPLETE_DIMENSIONS:
    //  return "GL_FRAMEBUFFER_INCOMPLETE_DIMENSIONS";
    case GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT:
        return "GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT";
    case GL_FRAMEBUFFER_UNSUPPORTED:
        return "GL_FRAMEBUFFER_UNSUPPORTED";
    }

    stringstream text;
    text << "[GL framebuffer status " << hex << status << "]";
    return text.str();
}

//-----------------------------------------------------------------------------

std::string GLDRAWBUFFERtoString( GLenum buffer ) {
    switch (buffer) {
    case GL_NONE:
        return "GL_NONE";
    case GL_FRONT_LEFT:
        return "GL_FRONT_LEFT";
    case GL_FRONT_RIGHT:
        return "GL_FRONT_RIGHT";
    case GL_BACK_LEFT:
        return "GL_BACK_LEFT";
    case GL_BACK_RIGHT:
        return "GL_BACK_RIGHT";
    case GL_FRONT:
        return "GL_FRONT";
    case GL_BACK:
        return "GL_BACK";
    case GL_LEFT:
        return "GL_LEFT";
    case GL_RIGHT:
        return "GL_RIGHT";
    case GL_FRONT_AND_BACK:
        return "GL_FRONT_AND_BACK";
    default:
        if (buffer >= GL_AUX0) {
            GLint numBuffers( 0 );
            glGetIntegerv( GL_AUX_BUFFERS, &numBuffers );
            if (buffer < static_cast<GLenum>(numBuffers)) {
                stringstream text( "GL_AUX" );
                text << (buffer - GL_AUX0);
                return text.str();
            }
        }
        return "";
    }
}

//-----------------------------------------------------------------------------
