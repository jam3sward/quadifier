#ifndef hive_IDirect3D9Proxy_h
#define hive_IDirect3D9Proxy_h

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

//-----------------------------------------------------------------------------

/**
 * Proxy class to replace the normal IDirect3D9 interface.
 */
class IDirect3D9Proxy: public IDirect3D9 {
public:
    IDirect3D9Proxy( LPDIRECT3D9 direct3D );

    virtual ~IDirect3D9Proxy();

    //--- IUnknown methods ----------------------------------------------------
    
    STDMETHOD(QueryInterface)(THIS_ REFIID riid, void** ppvObj);
    STDMETHOD_(ULONG,AddRef)(THIS);
    STDMETHOD_(ULONG,Release)(THIS);

    //--- IDirect3D9 methods --------------------------------------------------
    
    STDMETHOD(RegisterSoftwareDevice)(THIS_ void* pInitializeFunction);
    STDMETHOD_(UINT, GetAdapterCount)(THIS);
    STDMETHOD(GetAdapterIdentifier)(THIS_ UINT Adapter,DWORD Flags,D3DADAPTER_IDENTIFIER9* pIdentifier);
    STDMETHOD_(UINT, GetAdapterModeCount)(THIS_ UINT Adapter,D3DFORMAT Format);
    STDMETHOD(EnumAdapterModes)(THIS_ UINT Adapter,D3DFORMAT Format,UINT Mode,D3DDISPLAYMODE* pMode);
    STDMETHOD(GetAdapterDisplayMode)(THIS_ UINT Adapter,D3DDISPLAYMODE* pMode);
    STDMETHOD(CheckDeviceType)(THIS_ UINT Adapter,D3DDEVTYPE DevType,D3DFORMAT AdapterFormat,D3DFORMAT BackBufferFormat,BOOL bWindowed);
    STDMETHOD(CheckDeviceFormat)(THIS_ UINT Adapter,D3DDEVTYPE DeviceType,D3DFORMAT AdapterFormat,DWORD Usage,D3DRESOURCETYPE RType,D3DFORMAT CheckFormat);
    STDMETHOD(CheckDeviceMultiSampleType)(THIS_ UINT Adapter,D3DDEVTYPE DeviceType,D3DFORMAT SurfaceFormat,BOOL Windowed,D3DMULTISAMPLE_TYPE MultiSampleType,DWORD* pQualityLevels);
    STDMETHOD(CheckDepthStencilMatch)(THIS_ UINT Adapter,D3DDEVTYPE DeviceType,D3DFORMAT AdapterFormat,D3DFORMAT RenderTargetFormat,D3DFORMAT DepthStencilFormat);
    STDMETHOD(CheckDeviceFormatConversion)(THIS_ UINT Adapter,D3DDEVTYPE DeviceType,D3DFORMAT SourceFormat,D3DFORMAT TargetFormat);
    STDMETHOD(GetDeviceCaps)(THIS_ UINT Adapter,D3DDEVTYPE DeviceType,D3DCAPS9* pCaps);
    STDMETHOD_(HMONITOR, GetAdapterMonitor)(THIS_ UINT Adapter);
    STDMETHOD(CreateDevice)(THIS_ UINT Adapter,D3DDEVTYPE DeviceType,HWND hFocusWindow,DWORD BehaviorFlags,D3DPRESENT_PARAMETERS* pPresentationParameters,IDirect3DDevice9** ppReturnedDeviceInterface);

private:
    IDirect3D9 *m_parent;
};

//-----------------------------------------------------------------------------

#endif//hive_IDirect3D9Proxy_h
