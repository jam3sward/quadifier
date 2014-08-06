#if !defined(hive_DXGISwapChainProxy_h) && (defined(SUPPORT_D3D11))
#define hive_DXGISwapChainProxy_h

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

#include <d3d11.h>

//-----------------------------------------------------------------------------

struct DXGISwapChainProxy : public IDXGISwapChain {
public:
    DXGISwapChainProxy( IDXGISwapChain *chain );

    virtual ~DXGISwapChainProxy();

    //--- IUnknown methods ----------------------------------------------------
    
    STDMETHOD(QueryInterface)(THIS_ REFIID riid, void** ppvObj);

    STDMETHOD_(ULONG,AddRef)(THIS);

    STDMETHOD_(ULONG,Release)(THIS);

    //--- IDXGISwapChain methods ----------------------------------------------

    virtual HRESULT STDMETHODCALLTYPE Present(
        UINT SyncInterval,
        UINT Flags
    );

    virtual HRESULT STDMETHODCALLTYPE GetBuffer(
        UINT Buffer,
        __in REFIID riid,
        __out void **ppSurface
    );

    virtual HRESULT STDMETHODCALLTYPE SetFullscreenState(
        BOOL Fullscreen,
        __in_opt IDXGIOutput *pTarget
    );

    virtual HRESULT STDMETHODCALLTYPE GetFullscreenState(
        __out BOOL *pFullscreen,
        __out IDXGIOutput **ppTarget
    );

    virtual HRESULT STDMETHODCALLTYPE GetDesc(
        __out DXGI_SWAP_CHAIN_DESC *pDesc
    );

    virtual HRESULT STDMETHODCALLTYPE ResizeBuffers(
        UINT BufferCount,
        UINT Width,
        UINT Height,
        DXGI_FORMAT NewFormat,
        UINT SwapChainFlags
    );

    virtual HRESULT STDMETHODCALLTYPE ResizeTarget(
        __in const DXGI_MODE_DESC *pNewTargetParameters
    );

    virtual HRESULT STDMETHODCALLTYPE GetContainingOutput(
        __out IDXGIOutput **ppOutput
    );

    virtual HRESULT STDMETHODCALLTYPE GetFrameStatistics(
        __out DXGI_FRAME_STATISTICS *pStats
    );

    virtual HRESULT STDMETHODCALLTYPE GetLastPresentCount(
        __out UINT *pLastPresentCount
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

    //--- IDXGIDeviceSubObject methods ----------------------------------------
    
    virtual HRESULT STDMETHODCALLTYPE GetDevice(
        __in  REFIID riid,
        __out  void **ppDevice
    );

private:
    IDXGISwapChain *m_chain;
};

//-----------------------------------------------------------------------------

#endif//hive_DXGISwapChainProxy_h
