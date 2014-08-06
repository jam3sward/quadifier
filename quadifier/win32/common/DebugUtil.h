#ifndef DebugUtil_h
#define DebugUtil_h

#include <string>
#include <Windows.h>
#include <d3d9.h>
#include <GL/GL.h>

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

std::string formatErrorMessage( DWORD error );

std::string GUIDtoString( const GUID & guid );

std::string GUIDtoObjectName( const GUID & guid );

std::string D3DUSAGEtoString( DWORD usage );

std::string D3DPOOLtoString( D3DPOOL pool );

std::string D3DFVFtoString( DWORD fvf );

std::string D3DFORMATtoString( D3DFORMAT format );

std::string D3DMULTISAMPLE_TYPEtoString( D3DMULTISAMPLE_TYPE type );

std::string getGLErrorString();

std::string GLFRAMEBUFFERSTATUStoString( GLenum status );

//-----------------------------------------------------------------------------

#endif//DebugUtil_h
