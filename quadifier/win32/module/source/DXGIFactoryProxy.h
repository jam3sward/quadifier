#if !defined(hive_DXGIFactoryProxy_h) && (defined(SUPPORT_D3D11))
#define hive_DXGIFactoryProxy_h

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
//	  2. If you use this software in a product, an acknowledgment in the
//    product documentation is required.
//
//    3. Altered source versions must be plainly marked as such, and must not
//    be misrepresented as being the original software.
//
//    4. This notice may not be removed or altered from any source
//    distribution.
//
//-----------------------------------------------------------------------------

#include <d3d11.h>

//-----------------------------------------------------------------------------

struct DXGIFactoryProxy : public IDXGIFactory {
public:
    DXGIFactoryProxy( IDXGIFactory *factory );

    //--- IUnknown methods ----------------------------------------------------
    
    STDMETHOD(QueryInterface)(THIS_ REFIID riid, void** ppvObj);

    STDMETHOD_(ULONG,AddRef)(THIS);

    STDMETHOD_(ULONG,Release)(THIS);

    //--- IDXGIFactory methods ------------------------------------------------

    virtual HRESULT STDMETHODCALLTYPE EnumAdapters(
        UINT Adapter,
        __out IDXGIAdapter **ppAdapter
    );

    virtual HRESULT STDMETHODCALLTYPE MakeWindowAssociation(
        HWND WindowHandle,
        UINT Flags
    );

    virtual HRESULT STDMETHODCALLTYPE GetWindowAssociation(
        __out HWND *pWindowHandle
    );

    virtual HRESULT STDMETHODCALLTYPE CreateSwapChain(
        __in IUnknown *pDevice,
        __in DXGI_SWAP_CHAIN_DESC *pDesc,
        __out IDXGISwapChain **ppSwapChain
    );

    virtual HRESULT STDMETHODCALLTYPE CreateSoftwareAdapter(
        HMODULE Module,
        __out IDXGIAdapter **ppAdapter
    );

    //--- IDXGIObject methods -------------------------------------------------
    
    virtual HRESULT STDMETHODCALLTYPE SetPrivateData(
        __in  REFGUID Name,
        UINT DataSize,
        __in_bcount(DataSize)  const void *pData
    );

    virtual HRESULT STDMETHODCALLTYPE SetPrivateDataInterface(
        __in  REFGUID Name,
        __in  const IUnknown *pUnknown
    );

    virtual HRESULT STDMETHODCALLTYPE GetPrivateData(
        __in  REFGUID Name,
        __inout  UINT *pDataSize,
        __out_bcount(*pDataSize)  void *pData
    );

    virtual HRESULT STDMETHODCALLTYPE GetParent(
        __in  REFIID riid,
        __out  void **ppParent
    );

protected:
    IDXGIFactory *m_factory;
};

//-----------------------------------------------------------------------------

#endif//hive_DXGIFactoryProxy_h
