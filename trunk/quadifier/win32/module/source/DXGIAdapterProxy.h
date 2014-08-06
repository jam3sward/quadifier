#if !defined(hive_DXGIAdapterProxy_h) && (defined(SUPPORT_D3D11))
#define hive_DXGIAdapterProxy_h

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
#include "DXGIFactoryProxy.h"

//-----------------------------------------------------------------------------

struct DXGIAdapterProxy : public IDXGIAdapter {
public:
    DXGIAdapterProxy(
        IDXGIAdapter *adapter
    );

    //--- IUnknown methods ----------------------------------------------------
    
    STDMETHOD(QueryInterface)(THIS_ REFIID riid, void** ppvObj);

    STDMETHOD_(ULONG,AddRef)(THIS);

    STDMETHOD_(ULONG,Release)(THIS);

    //--- ID3D11Adapter methods -----------------------------------------------

    virtual HRESULT STDMETHODCALLTYPE EnumOutputs( 
        UINT Output,
        __out IDXGIOutput **ppOutput
    );
        
    virtual HRESULT STDMETHODCALLTYPE GetDesc( 
        __out  DXGI_ADAPTER_DESC *pDesc
    );
        
    virtual HRESULT STDMETHODCALLTYPE CheckInterfaceSupport( 
        __in  REFGUID InterfaceName,
        __out  LARGE_INTEGER *pUMDVersion
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
    IDXGIAdapter *m_adapter;
    std::shared_ptr<DXGIFactoryProxy> m_dxgiFactoryProxy;
};

#endif//hive_DXGIAdapterProxy_h