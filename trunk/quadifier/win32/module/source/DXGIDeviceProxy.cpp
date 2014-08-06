#if defined(SUPPORT_D3D11)

#include "DXGIDeviceProxy.h"
#include "Log.h"
#include "DebugUtil.h"

using namespace hive;

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

DXGIDeviceProxy::DXGIDeviceProxy( IDXGIDevice *device ) :
    m_device( device )
{
}

//-----------------------------------------------------------------------------

HRESULT DXGIDeviceProxy::QueryInterface( REFIID riid, void **ppvObj)
{
    Log::print() << "ID3D11DeviceProxy::QueryInterface("
                 << GUIDtoObjectName(riid) << ")\n";
    
    HRESULT result = m_device->QueryInterface( riid, ppvObj );

    if ( (riid == __uuidof(IDXGIDevice)) && (result == S_OK) ) {
        *ppvObj = this;
    }

    return result;
}

//-----------------------------------------------------------------------------

ULONG DXGIDeviceProxy::AddRef()
{
    ULONG result = m_device->AddRef();
    Log::print() << "DXGIDeviceProxy::AddRef, device=" << m_device
                 << ", refcount=" << result << std::endl;
    return m_device->AddRef();
}

//-----------------------------------------------------------------------------

ULONG DXGIDeviceProxy::Release()
{
    if ( m_device != 0 ) {
        ULONG result = m_device->Release();
        Log::print() << "DXGIDeviceProxy::Release, device=" << m_device
            << ", refcount=" << result << std::endl;
        if ( result == 0 )
            m_device = 0;
        return result;
    } else {
        Log::print() << "\nerror: DXGIDeviceProxy::Release of null device\n"; 
        return 0;
    }
}

//-----------------------------------------------------------------------------

HRESULT DXGIDeviceProxy::GetAdapter( 
    __out IDXGIAdapter **pAdapter
) {
    return m_device->GetAdapter( pAdapter );
}

//-----------------------------------------------------------------------------

HRESULT STDMETHODCALLTYPE DXGIDeviceProxy::CreateSurface( 
    __in const DXGI_SURFACE_DESC *pDesc,
    UINT NumSurfaces,
    DXGI_USAGE Usage,
    __in_opt const DXGI_SHARED_RESOURCE *pSharedResource,
    __out IDXGISurface **ppSurface
) {
    return m_device->CreateSurface(
        pDesc,
        NumSurfaces,
        Usage,
        pSharedResource,
        ppSurface
    );
}

//-----------------------------------------------------------------------------

HRESULT STDMETHODCALLTYPE DXGIDeviceProxy::QueryResourceResidency( 
    __in_ecount(NumResources) IUnknown *const *ppResources,
    __out_ecount(NumResources) DXGI_RESIDENCY *pResidencyStatus,
    UINT NumResources
) {
    return m_device->QueryResourceResidency(
        ppResources,
        pResidencyStatus,
        NumResources
    );
}

//-----------------------------------------------------------------------------

HRESULT STDMETHODCALLTYPE DXGIDeviceProxy::SetGPUThreadPriority( 
    INT Priority
) {
    return m_device->SetGPUThreadPriority( Priority );
}

//-----------------------------------------------------------------------------

HRESULT STDMETHODCALLTYPE DXGIDeviceProxy::GetGPUThreadPriority( 
    __out  INT *pPriority
) {
    return m_device->GetGPUThreadPriority( pPriority );
}

//-----------------------------------------------------------------------------

HRESULT STDMETHODCALLTYPE DXGIDeviceProxy::GetPrivateData(
    __in REFGUID guid,
    __inout UINT *pDataSize,
    __out_bcount_opt(*pDataSize) void *pData
) {
    return m_device->GetPrivateData( guid, pDataSize, pData );
}

//-----------------------------------------------------------------------------

HRESULT STDMETHODCALLTYPE DXGIDeviceProxy::SetPrivateData(
    __in REFGUID guid,
    __in UINT DataSize,
    __in_bcount_opt(DataSize) const void *pData
) {
    return m_device->SetPrivateData( guid, DataSize, pData );
}

//-----------------------------------------------------------------------------

HRESULT STDMETHODCALLTYPE DXGIDeviceProxy::SetPrivateDataInterface(
    __in REFGUID guid,
    __in_opt const IUnknown *pData
) {
    return m_device->SetPrivateDataInterface( guid, pData );
}

//-----------------------------------------------------------------------------

HRESULT STDMETHODCALLTYPE DXGIDeviceProxy::GetParent(  
    __in  REFIID riid,
    __out  void **ppParent
) {
    Log::print() << "DXGIDeviceProxy::GetParent("
                << GUIDtoObjectName(riid) << ")\n";
    
    HRESULT result = m_device->GetParent( riid, ppParent );

    if ( (riid == __uuidof(IDXGIAdapter)) && (result == S_OK) ) {
        // create proxy IDXGIAdapter if needed
        if ( m_dxgiAdapterProxy == 0 ) {
            m_dxgiAdapterProxy.reset(
                new DXGIAdapterProxy( reinterpret_cast<IDXGIAdapter*>(*ppParent) )
            );
        }
        // substitute proxy IDXGIAdapter
        *ppParent = m_dxgiAdapterProxy.get();
    }

    return result;
}

//-----------------------------------------------------------------------------

#endif//SUPPORT_D3D11