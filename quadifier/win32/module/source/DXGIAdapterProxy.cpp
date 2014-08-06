#if defined(SUPPORT_D3D11)

#include "DXGIAdapterProxy.h"
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

DXGIAdapterProxy::DXGIAdapterProxy( IDXGIAdapter *adapter ) :
    m_adapter( adapter )
{
}

//-----------------------------------------------------------------------------

HRESULT DXGIAdapterProxy::QueryInterface( REFIID riid, void **ppvObj)
{
    Log::print() << "DXGIAdapterProxy::QueryInterface("
                 << GUIDtoObjectName(riid) << ")\n";
    
    HRESULT result = m_adapter->QueryInterface( riid, ppvObj );

    if ( (riid == __uuidof(IDXGIAdapter)) && (result == S_OK) ) {
        *ppvObj = this;
    }

    return result; 
}

//-----------------------------------------------------------------------------

ULONG DXGIAdapterProxy::AddRef()
{
    return m_adapter->AddRef();
}

//-----------------------------------------------------------------------------

ULONG DXGIAdapterProxy::Release()
{
    return m_adapter->Release();
}

//-----------------------------------------------------------------------------

HRESULT STDMETHODCALLTYPE DXGIAdapterProxy::EnumOutputs( 
    UINT Output,
    __out IDXGIOutput **ppOutput
) {
    return m_adapter->EnumOutputs( Output, ppOutput );
}

//-----------------------------------------------------------------------------

HRESULT STDMETHODCALLTYPE DXGIAdapterProxy::GetDesc( 
    __out  DXGI_ADAPTER_DESC *pDesc
) {
    return m_adapter->GetDesc( pDesc );
}

//-----------------------------------------------------------------------------

HRESULT STDMETHODCALLTYPE DXGIAdapterProxy::CheckInterfaceSupport( 
    __in  REFGUID InterfaceName,
    __out  LARGE_INTEGER *pUMDVersion
) {
    return m_adapter->CheckInterfaceSupport( InterfaceName, pUMDVersion );
}

//-----------------------------------------------------------------------------

HRESULT STDMETHODCALLTYPE DXGIAdapterProxy::GetPrivateData(
    __in REFGUID guid,
    __inout UINT *pDataSize,
    __out_bcount_opt(*pDataSize) void *pData
) {
    return m_adapter->GetPrivateData( guid, pDataSize, pData );
}

//-----------------------------------------------------------------------------

HRESULT STDMETHODCALLTYPE DXGIAdapterProxy::SetPrivateData(
    __in REFGUID guid,
    __in UINT DataSize,
    __in_bcount_opt(DataSize) const void *pData
) {
    return m_adapter->SetPrivateData( guid, DataSize, pData );
}

//-----------------------------------------------------------------------------

HRESULT STDMETHODCALLTYPE DXGIAdapterProxy::SetPrivateDataInterface(
    __in REFGUID guid,
    __in_opt const IUnknown *pData
) {
    return m_adapter->SetPrivateDataInterface( guid, pData );
}

//-----------------------------------------------------------------------------

HRESULT STDMETHODCALLTYPE DXGIAdapterProxy::GetParent(  
    __in  REFIID riid,
    __out  void **ppParent
) {
    Log::print() << "DXGIAdapterProxy::GetParent("
                << GUIDtoObjectName(riid) << ")\n";
    
    HRESULT result = m_adapter->GetParent( riid, ppParent );

    if ( (riid == __uuidof(IDXGIFactory)) && (result == S_OK) ) {
        // create proxy IDXGIFactory if needed
        if ( m_dxgiFactoryProxy == 0 ) {
            m_dxgiFactoryProxy.reset(
                new DXGIFactoryProxy( reinterpret_cast<IDXGIFactory*>(*ppParent) )
            );
        }
        // substitute proxy IDXGIFactory
        //*ppParent = m_dxgiFactoryProxy.get();
    }

    return result;
}

//-----------------------------------------------------------------------------

#endif//SUPPORT_D3D11