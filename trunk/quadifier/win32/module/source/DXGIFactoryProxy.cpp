#if defined(SUPPORT_D3D11)

#include "DXGIFactoryProxy.h"
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

DXGIFactoryProxy::DXGIFactoryProxy(
    IDXGIFactory *factory
)
    : m_factory( factory )
{
    Log::print() << "DXGIFactoryProxy(" << factory << ")\n";
}

//-----------------------------------------------------------------------------

HRESULT DXGIFactoryProxy::QueryInterface( REFIID riid, void **ppvObj)
{
    Log::print() << "DXGIFactoryProxy::QueryInterface("
                 << GUIDtoObjectName(riid) << ")\n";

    HRESULT result = m_factory->QueryInterface( riid, ppvObj );

    if ( (riid == __uuidof(IDXGIFactory)) && (result == S_OK) ) {
        *ppvObj = this;
    }

    return result; 
}

//-----------------------------------------------------------------------------

ULONG DXGIFactoryProxy::AddRef()
{
    return m_factory->AddRef();
}

//-----------------------------------------------------------------------------

ULONG DXGIFactoryProxy::Release()
{
    return m_factory->Release();
}

//-----------------------------------------------------------------------------

HRESULT STDMETHODCALLTYPE DXGIFactoryProxy::EnumAdapters(
    UINT Adapter,
    __out IDXGIAdapter **ppAdapter
) {
    return m_factory->EnumAdapters( Adapter, ppAdapter );
}

//-----------------------------------------------------------------------------

HRESULT STDMETHODCALLTYPE DXGIFactoryProxy::MakeWindowAssociation(
    HWND WindowHandle,
    UINT Flags
) {
    return m_factory->MakeWindowAssociation( WindowHandle, Flags );
}

//-----------------------------------------------------------------------------

HRESULT STDMETHODCALLTYPE DXGIFactoryProxy::GetWindowAssociation(
    __out HWND *pWindowHandle
) {
    return m_factory->GetWindowAssociation( pWindowHandle );
}

//-----------------------------------------------------------------------------

HRESULT STDMETHODCALLTYPE DXGIFactoryProxy::CreateSwapChain(
    __in IUnknown *pDevice,
    __in DXGI_SWAP_CHAIN_DESC *pDesc,
    __out IDXGISwapChain **ppSwapChain
) {
    return m_factory->CreateSwapChain( pDevice, pDesc, ppSwapChain );
}

//-----------------------------------------------------------------------------

HRESULT STDMETHODCALLTYPE DXGIFactoryProxy::CreateSoftwareAdapter(
    HMODULE Module,
    __out IDXGIAdapter **ppAdapter
) {
    return m_factory->CreateSoftwareAdapter( Module, ppAdapter );
}

//-----------------------------------------------------------------------------

HRESULT STDMETHODCALLTYPE DXGIFactoryProxy::SetPrivateData(
    __in  REFGUID Name,
    UINT DataSize,
    __in_bcount(DataSize)  const void *pData
) {
    return m_factory->SetPrivateData( Name, DataSize, pData );
}

//-----------------------------------------------------------------------------

HRESULT STDMETHODCALLTYPE DXGIFactoryProxy::SetPrivateDataInterface(
    __in  REFGUID Name,
    __in  const IUnknown *pUnknown
) {
    return m_factory->SetPrivateDataInterface( Name, pUnknown );
}

//-----------------------------------------------------------------------------

HRESULT STDMETHODCALLTYPE DXGIFactoryProxy::GetPrivateData(
    __in  REFGUID Name,
    __inout  UINT *pDataSize,
    __out_bcount(*pDataSize) void *pData
) {
    return m_factory->GetPrivateData( Name, pDataSize, pData );
}

//-----------------------------------------------------------------------------

HRESULT STDMETHODCALLTYPE DXGIFactoryProxy::GetParent(
    __in  REFIID riid,
    __out  void **ppParent
) {
    Log::print() << "DXGIFactoryProxy::GetParent("
                << GUIDtoObjectName(riid) << ")\n";
    return m_factory->GetParent( riid, ppParent );
}

//-----------------------------------------------------------------------------

#endif//SUPPORT_D3D11