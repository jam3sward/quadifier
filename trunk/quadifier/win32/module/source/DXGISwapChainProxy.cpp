#if defined(SUPPORT_D3D11)

#include "DXGISwapChainProxy.h"
#include "Log.h"
#include "DebugUtil.h"

using namespace hive;

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

DXGISwapChainProxy::DXGISwapChainProxy(
    IDXGISwapChain *chain
)
    : m_chain( chain )
{
    Log::print() << "DXGISwapChainProxy(" << chain << ")\n";
}

//-----------------------------------------------------------------------------

DXGISwapChainProxy::~DXGISwapChainProxy()
{
}

//-----------------------------------------------------------------------------

HRESULT DXGISwapChainProxy::QueryInterface( REFIID riid, void **ppvObj)
{
    Log::print() << "DXGISwapChainProxy::QueryInterface("
                 << GUIDtoObjectName(riid) << ")\n";
    HRESULT result = m_chain->QueryInterface( riid, ppvObj ); 
    return result; 
}

//-----------------------------------------------------------------------------

ULONG DXGISwapChainProxy::AddRef()
{
    return m_chain->AddRef();
}

//-----------------------------------------------------------------------------

ULONG DXGISwapChainProxy::Release()
{
    return m_chain->Release();
}

//-----------------------------------------------------------------------------

HRESULT STDMETHODCALLTYPE DXGISwapChainProxy::Present(
    UINT SyncInterval,
    UINT Flags
) {
    return m_chain->Present( SyncInterval, Flags );
}

//-----------------------------------------------------------------------------

HRESULT STDMETHODCALLTYPE DXGISwapChainProxy::GetBuffer(
    UINT Buffer,
    __in REFIID riid,
    __out void **ppSurface
) {
    return m_chain->GetBuffer( Buffer, riid, ppSurface );
}

//-----------------------------------------------------------------------------

HRESULT STDMETHODCALLTYPE DXGISwapChainProxy::SetFullscreenState(
    BOOL Fullscreen,
    __in_opt IDXGIOutput *pTarget
) {
    return m_chain->SetFullscreenState( Fullscreen, pTarget );
}

//-----------------------------------------------------------------------------

HRESULT STDMETHODCALLTYPE DXGISwapChainProxy::GetFullscreenState(
    __out BOOL *pFullscreen,
    __out IDXGIOutput **ppTarget
) {
    return m_chain->GetFullscreenState( pFullscreen, ppTarget );
}

//-----------------------------------------------------------------------------

HRESULT STDMETHODCALLTYPE DXGISwapChainProxy::GetDesc(
    __out DXGI_SWAP_CHAIN_DESC *pDesc
) {
    return m_chain->GetDesc( pDesc );
}

//-----------------------------------------------------------------------------

HRESULT STDMETHODCALLTYPE DXGISwapChainProxy::ResizeBuffers(
    UINT BufferCount,
    UINT Width,
    UINT Height,
    DXGI_FORMAT NewFormat,
    UINT SwapChainFlags
) {
    return m_chain->ResizeBuffers(
        BufferCount,
        Width,
        Height,
        NewFormat,
        SwapChainFlags
    );
}

//-----------------------------------------------------------------------------

HRESULT STDMETHODCALLTYPE DXGISwapChainProxy::ResizeTarget(
    __in const DXGI_MODE_DESC *pNewTargetParameters
) {
    return m_chain->ResizeTarget( pNewTargetParameters );
}

//-----------------------------------------------------------------------------

HRESULT STDMETHODCALLTYPE DXGISwapChainProxy::GetContainingOutput(
    __out IDXGIOutput **ppOutput
) {
    return m_chain->GetContainingOutput( ppOutput );
}

//-----------------------------------------------------------------------------

HRESULT STDMETHODCALLTYPE DXGISwapChainProxy::GetFrameStatistics(
    __out DXGI_FRAME_STATISTICS *pStats
) {
    return m_chain->GetFrameStatistics( pStats );
}

//-----------------------------------------------------------------------------

HRESULT STDMETHODCALLTYPE DXGISwapChainProxy::GetLastPresentCount(
    __out UINT *pLastPresentCount
) {
    return m_chain->GetLastPresentCount( pLastPresentCount );
}

//-----------------------------------------------------------------------------

HRESULT STDMETHODCALLTYPE DXGISwapChainProxy::SetPrivateData(
    __in  REFGUID Name,
    UINT DataSize,
    __in_bcount(DataSize)  const void *pData
) {
    return m_chain->SetPrivateData( Name, DataSize, pData );
}

//-----------------------------------------------------------------------------

HRESULT STDMETHODCALLTYPE DXGISwapChainProxy::SetPrivateDataInterface(
    __in  REFGUID Name,
    __in  const IUnknown *pUnknown
) {
    return m_chain->SetPrivateDataInterface( Name, pUnknown );
}

//-----------------------------------------------------------------------------

HRESULT STDMETHODCALLTYPE DXGISwapChainProxy::GetPrivateData(
    __in  REFGUID Name,
    __inout  UINT *pDataSize,
    __out_bcount(*pDataSize) void *pData
) {
    return m_chain->GetPrivateData( Name, pDataSize, pData );
}

//-----------------------------------------------------------------------------

HRESULT STDMETHODCALLTYPE DXGISwapChainProxy::GetParent(
    __in  REFIID riid,
    __out  void **ppParent
) {
    return m_chain->GetParent( riid, ppParent );
}

//-----------------------------------------------------------------------------

HRESULT STDMETHODCALLTYPE DXGISwapChainProxy::GetDevice(
    __in  REFIID riid,
    __out  void **ppDevice
) {
    return m_chain->GetDevice( riid, ppDevice );
}

//-----------------------------------------------------------------------------

#endif//SUPPORT_D3D11