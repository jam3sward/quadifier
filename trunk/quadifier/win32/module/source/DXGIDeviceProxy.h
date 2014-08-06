#if !defined(hive_DXGIDeviceProxy_h) && (defined(SUPPORT_D3D11))
#define hive_DXGIDeviceProxy_h

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

#include <memory>
#include <d3d11.h>
#include "DXGIAdapterProxy.h"

//-----------------------------------------------------------------------------

struct DXGIDeviceProxy : public IDXGIDevice {
public:
    DXGIDeviceProxy( IDXGIDevice *device );

    //--- IUnknown methods ----------------------------------------------------
    
    STDMETHOD(QueryInterface)(THIS_ REFIID riid, void** ppvObj);

    STDMETHOD_(ULONG,AddRef)(THIS);

    STDMETHOD_(ULONG,Release)(THIS);

    //--- IDXGIDevice methods -------------------------------------------------

    virtual HRESULT STDMETHODCALLTYPE GetAdapter( 
        __out IDXGIAdapter **pAdapter
    );

    virtual HRESULT STDMETHODCALLTYPE CreateSurface( 
        __in const DXGI_SURFACE_DESC *pDesc,
        UINT NumSurfaces,
        DXGI_USAGE Usage,
        __in_opt const DXGI_SHARED_RESOURCE *pSharedResource,
        __out IDXGISurface **ppSurface
    );

    virtual HRESULT STDMETHODCALLTYPE QueryResourceResidency( 
        __in_ecount(NumResources) IUnknown *const *ppResources,
        __out_ecount(NumResources) DXGI_RESIDENCY *pResidencyStatus,
        UINT NumResources
    );

    virtual HRESULT STDMETHODCALLTYPE SetGPUThreadPriority( 
        INT Priority
    );

    virtual HRESULT STDMETHODCALLTYPE GetGPUThreadPriority( 
        __out  INT *pPriority
    );

    //--- IDXGIObject methods -------------------------------------------------

    virtual HRESULT STDMETHODCALLTYPE GetPrivateData(
        __in REFGUID guid,
        __inout UINT *pDataSize,
        __out_bcount_opt( *pDataSize ) void *pData
    );

    virtual HRESULT STDMETHODCALLTYPE SetPrivateData(
        __in REFGUID guid,
        __in UINT DataSize,
        __in_bcount_opt( DataSize ) const void *pData
    );

    virtual HRESULT STDMETHODCALLTYPE SetPrivateDataInterface(
        __in REFGUID guid,
        __in_opt const IUnknown *pData
    );

    virtual HRESULT STDMETHODCALLTYPE GetParent(  
        __in  REFIID riid,
        __out  void **ppParent
    );

private:
    IDXGIDevice *m_device;
    std::shared_ptr<DXGIAdapterProxy> m_dxgiAdapterProxy;
};

//-----------------------------------------------------------------------------

#endif//hive_DXGIDeviceProxy_h
