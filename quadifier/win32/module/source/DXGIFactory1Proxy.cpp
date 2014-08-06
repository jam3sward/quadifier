#if defined(SUPPORT_D3D11)

#include "DXGIFactory1Proxy.h"
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

DXGIFactory1Proxy::DXGIFactory1Proxy(
    IDXGIFactory1 *factory
)
    : m_factory( factory )
{
    Log::print() << "DXGIFactory1Proxy(" << factory << ")\n";
}

//-----------------------------------------------------------------------------

HRESULT DXGIFactory1Proxy::QueryInterface( REFIID riid, void **ppvObj)
{
    Log::print() << "DXGIFactory1Proxy::QueryInterface("
                 << GUIDtoObjectName(riid) << ")\n";
    HRESULT result = m_factory->QueryInterface( riid, ppvObj ); 
    return result; 
}

//-----------------------------------------------------------------------------

ULONG DXGIFactory1Proxy::AddRef()
{
    return m_factory->AddRef();
}

//-----------------------------------------------------------------------------

ULONG DXGIFactory1Proxy::Release()
{
    return m_factory->Release();
}

//-----------------------------------------------------------------------------

HRESULT STDMETHODCALLTYPE DXGIFactory1Proxy::EnumAdapters(
    UINT Adapter,
    __out IDXGIAdapter **ppAdapter
) {
    return m_factory->EnumAdapters( Adapter, ppAdapter );
}

//-----------------------------------------------------------------------------

HRESULT STDMETHODCALLTYPE DXGIFactory1Proxy::MakeWindowAssociation(
    HWND WindowHandle,
    UINT Flags
) {
    return m_factory->MakeWindowAssociation( WindowHandle, Flags );
}

//-----------------------------------------------------------------------------

HRESULT STDMETHODCALLTYPE DXGIFactory1Proxy::GetWindowAssociation(
    __out HWND *pWindowHandle
) {
    return m_factory->GetWindowAssociation( pWindowHandle );
}

//-----------------------------------------------------------------------------

HRESULT STDMETHODCALLTYPE DXGIFactory1Proxy::CreateSwapChain(
    __in IUnknown *pDevice,
    __in DXGI_SWAP_CHAIN_DESC *pDesc,
    __out IDXGISwapChain **ppSwapChain
) {
    HRESULT result = m_factory->CreateSwapChain( pDevice, pDesc, ppSwapChain );
    Log::print() << "CreateSwapChain(" << pDevice << ',' << pDesc << ',' << ppSwapChain << ")"
        << " = " << result << ',' << *ppSwapChain << '\n';
    return result;
}

//-----------------------------------------------------------------------------

HRESULT STDMETHODCALLTYPE DXGIFactory1Proxy::CreateSoftwareAdapter(
    HMODULE Module,
    __out IDXGIAdapter **ppAdapter
) {
    return m_factory->CreateSoftwareAdapter( Module, ppAdapter );
}

//-----------------------------------------------------------------------------

HRESULT STDMETHODCALLTYPE DXGIFactory1Proxy::SetPrivateData(
    __in  REFGUID Name,
    UINT DataSize,
    __in_bcount(DataSize)  const void *pData
) {
    return m_factory->SetPrivateData( Name, DataSize, pData );
}

//-----------------------------------------------------------------------------

HRESULT STDMETHODCALLTYPE DXGIFactory1Proxy::SetPrivateDataInterface(
    __in  REFGUID Name,
    __in  const IUnknown *pUnknown
) {
    return m_factory->SetPrivateDataInterface( Name, pUnknown );
}

//-----------------------------------------------------------------------------

HRESULT STDMETHODCALLTYPE DXGIFactory1Proxy::GetPrivateData(
    __in  REFGUID Name,
    __inout  UINT *pDataSize,
    __out_bcount(*pDataSize) void *pData
) {
    return m_factory->GetPrivateData( Name, pDataSize, pData );
}

//-----------------------------------------------------------------------------

HRESULT STDMETHODCALLTYPE DXGIFactory1Proxy::GetParent(
    __in  REFIID riid,
    __out  void **ppParent
) {
    return m_factory->GetParent( riid, ppParent );
}

//-----------------------------------------------------------------------------

HRESULT STDMETHODCALLTYPE DXGIFactory1Proxy::EnumAdapters1(
    UINT Adapter,
    __out IDXGIAdapter1 **ppAdapter
) {
    return dynamic_cast<IDXGIFactory1*>(m_factory)->EnumAdapters1(
        Adapter, ppAdapter
    );
}

//-----------------------------------------------------------------------------

BOOL STDMETHODCALLTYPE DXGIFactory1Proxy::IsCurrent()
{
    return dynamic_cast<IDXGIFactory1*>(m_factory)->IsCurrent();
}

//-----------------------------------------------------------------------------

#endif//SUPPORT_D3D11