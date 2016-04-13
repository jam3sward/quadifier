#include "IDirect3D9Proxy.h"
#include "IDirect3DDevice9Proxy.h"
#include "Log.h"
#include "DebugUtil.h"
#include "Settings.h"
using namespace hive;
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

IDirect3D9Proxy::IDirect3D9Proxy( LPDIRECT3D9 direct3D ) :
    m_parent( direct3D )
{
    if (Log::info())
        Log::print() << "IDirect3D9Proxy(" << direct3D << ")\n";
}

//-----------------------------------------------------------------------------

IDirect3D9Proxy::~IDirect3D9Proxy()
{
}

//-----------------------------------------------------------------------------

HRESULT IDirect3D9Proxy::QueryInterface( const IID &riid, void **ppvObj )
{
    if (Log::info()) {
        Log::print() << "IDirect3D9Proxy::QueryInterface("
                    << GUIDtoObjectName(riid) << ")\n";
    }

    return m_parent->QueryInterface(riid, ppvObj);
}

//-----------------------------------------------------------------------------

ULONG IDirect3D9Proxy::AddRef()
{
   return m_parent->AddRef();
}

//-----------------------------------------------------------------------------

ULONG IDirect3D9Proxy::Release()
{
   return m_parent->Release();
}

//-----------------------------------------------------------------------------

HRESULT IDirect3D9Proxy::RegisterSoftwareDevice( void *pInitializeFunction )
{
    return m_parent->RegisterSoftwareDevice( pInitializeFunction );
}

//-----------------------------------------------------------------------------

UINT IDirect3D9Proxy::GetAdapterCount()
{
    return m_parent->GetAdapterCount();
}

//-----------------------------------------------------------------------------

HRESULT IDirect3D9Proxy::GetAdapterIdentifier(
    UINT Adapter,
    DWORD Flags,
    D3DADAPTER_IDENTIFIER9 *pIdentifier
) {
    return m_parent->GetAdapterIdentifier( Adapter, Flags, pIdentifier );
}

//-----------------------------------------------------------------------------

UINT IDirect3D9Proxy::GetAdapterModeCount( UINT Adapter, D3DFORMAT Format )
{
    return m_parent->GetAdapterModeCount( Adapter, Format );
}

//-----------------------------------------------------------------------------

HRESULT IDirect3D9Proxy::EnumAdapterModes(
    UINT Adapter,
    D3DFORMAT Format,
    UINT Mode,
    D3DDISPLAYMODE *pMode
) {
    return m_parent->EnumAdapterModes( Adapter, Format, Mode, pMode );
}

//-----------------------------------------------------------------------------

HRESULT IDirect3D9Proxy::GetAdapterDisplayMode(
    UINT Adapter,
    D3DDISPLAYMODE *pMode
) {
    return m_parent->GetAdapterDisplayMode( Adapter, pMode );
}

//-----------------------------------------------------------------------------

HRESULT IDirect3D9Proxy::CheckDeviceType(
    UINT Adapter,
    D3DDEVTYPE DevType,
    D3DFORMAT AdapterFormat,
    D3DFORMAT BackBufferFormat,
    BOOL bWindowed
) {
    return m_parent->CheckDeviceType(
        Adapter, DevType, AdapterFormat, BackBufferFormat, bWindowed
    );
}

//-----------------------------------------------------------------------------

HRESULT IDirect3D9Proxy::CheckDeviceFormat(
    UINT Adapter,
    D3DDEVTYPE DeviceType,
    D3DFORMAT AdapterFormat,
    DWORD Usage,
    D3DRESOURCETYPE RType,
    D3DFORMAT CheckFormat
) {
    return m_parent->CheckDeviceFormat(
        Adapter, DeviceType, AdapterFormat, Usage, RType, CheckFormat
    );
}

//-----------------------------------------------------------------------------

HRESULT IDirect3D9Proxy::CheckDeviceMultiSampleType(
    UINT Adapter,
    D3DDEVTYPE DeviceType,
    D3DFORMAT SurfaceFormat,
    BOOL Windowed,
    D3DMULTISAMPLE_TYPE MultiSampleType,
    DWORD *pQualityLevels
) {
    return m_parent->CheckDeviceMultiSampleType(
        Adapter, DeviceType, SurfaceFormat, Windowed,
        MultiSampleType, pQualityLevels
    );
}

//-----------------------------------------------------------------------------

HRESULT IDirect3D9Proxy::CheckDepthStencilMatch(
    UINT Adapter,
    D3DDEVTYPE DeviceType,
    D3DFORMAT AdapterFormat,
    D3DFORMAT RenderTargetFormat,
    D3DFORMAT DepthStencilFormat
) {
    return m_parent->CheckDepthStencilMatch(
        Adapter, DeviceType, AdapterFormat,
        RenderTargetFormat, DepthStencilFormat
    );
}

//-----------------------------------------------------------------------------

HRESULT IDirect3D9Proxy::CheckDeviceFormatConversion(
    UINT Adapter,
    D3DDEVTYPE DeviceType,
    D3DFORMAT SourceFormat,
    D3DFORMAT TargetFormat
) {
    return m_parent->CheckDeviceFormatConversion(
        Adapter, DeviceType, SourceFormat, TargetFormat
    );
}

//-----------------------------------------------------------------------------

HRESULT IDirect3D9Proxy::GetDeviceCaps(
    UINT Adapter,
    D3DDEVTYPE DeviceType,
    D3DCAPS9 *pCaps
) {
    return m_parent->GetDeviceCaps( Adapter, DeviceType, pCaps );
}

//-----------------------------------------------------------------------------

HMONITOR IDirect3D9Proxy::GetAdapterMonitor( UINT Adapter )
{
    return m_parent->GetAdapterMonitor( Adapter );
}

//-----------------------------------------------------------------------------

HRESULT IDirect3D9Proxy::CreateDevice(
    UINT Adapter,
    D3DDEVTYPE DeviceType,
    HWND hFocusWindow,
    DWORD BehaviorFlags,
    D3DPRESENT_PARAMETERS *pPresentationParameters,
    IDirect3DDevice9 **ppReturnedDeviceInterface
) {
    if (Log::info()) {
        Log::print() << "CreateDevice(" << Adapter << ','
            << DeviceType << ','
            << hFocusWindow << ','
            << BehaviorFlags << ','
            << pPresentationParameters << ','
            << ppReturnedDeviceInterface << ")\n";
    }

    HRESULT result = 0;

    IDirect3DDevice9 *device = 0; 

    if ( !Settings::get().forceDirect3D9Ex ) {
        // using conventional Direct3D9

        if (Log::info())
            Log::print() << "using Direct3D9\n";

        result = m_parent->CreateDevice(
            Adapter,
            DeviceType,
            hFocusWindow,
            BehaviorFlags,
            pPresentationParameters,
            &device
        );
    } else { 
        // using Direct3D9Ex instead (substitution)

        if (Log::info())
            Log::print() << "using Direct3D9Ex\n";

        // upcast parent to IDirect3D9Ex
        IDirect3D9Ex *parent = reinterpret_cast<IDirect3D9Ex*>(m_parent);

        // create IDirect3DDevice9Ex
        // note: must be multithreaded for use with NV_DX_interop
        IDirect3DDevice9Ex *deviceEx = 0;
        result = parent->CreateDeviceEx(
            Adapter,
            DeviceType,
            hFocusWindow,
            BehaviorFlags | D3DCREATE_MULTITHREADED,
            pPresentationParameters,
            0,
            &deviceEx
        );

        device = reinterpret_cast<IDirect3DDevice9*>(deviceEx);
    }

    *ppReturnedDeviceInterface = new IDirect3DDevice9Proxy( device, this );
    if (Log::info()) {
        Log::print() << "new IDirect3DDevice9Proxy(" << device << ',' << this
            << ")=" << *ppReturnedDeviceInterface << endl;
    }

    return result;
}

//-----------------------------------------------------------------------------
