#if defined(SUPPORT_D3D11)

#include "ID3D11DeviceProxy.h"
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

ID3D11DeviceProxy::ID3D11DeviceProxy(
    ID3D11Device *device
) :
    m_device( device ),
    m_dxgiDeviceProxy( 0 )
{
    Log::print() << "ID3D11DeviceProxy(" << device << ")\n";
}

//-----------------------------------------------------------------------------

ID3D11DeviceProxy::~ID3D11DeviceProxy() {
}

//-----------------------------------------------------------------------------

HRESULT ID3D11DeviceProxy::QueryInterface( REFIID riid, void **ppvObj)
{
    Log::print() << "ID3D11DeviceProxy::QueryInterface("
                 << GUIDtoObjectName(riid) << ")\n";

    HRESULT result = m_device->QueryInterface( riid, ppvObj );

    if ( result == S_OK ) {
        if ( riid == __uuidof(ID3D11Device) ) {
            // substitute our proxy device
            *ppvObj = this;
        } else if ( riid == __uuidof(IDXGIDevice) ) {
            // create proxy IDXGIDevice if needed
            if ( m_dxgiDeviceProxy == 0 ) {
                m_dxgiDeviceProxy.reset(
                    new DXGIDeviceProxy( reinterpret_cast<IDXGIDevice*>(*ppvObj) )
                );
            }
            // substitute proxy IDXGIDevice
            *ppvObj = m_dxgiDeviceProxy.get();
        }
    }

    return result; 
}

//-----------------------------------------------------------------------------

ULONG ID3D11DeviceProxy::AddRef()
{
    return m_device->AddRef();
}

//-----------------------------------------------------------------------------

ULONG ID3D11DeviceProxy::Release()
{
    return m_device->Release();
}

//-----------------------------------------------------------------------------

HRESULT STDMETHODCALLTYPE ID3D11DeviceProxy::CreateBuffer(
    __in const D3D11_BUFFER_DESC *pDesc,
    __in_opt const D3D11_SUBRESOURCE_DATA *pInitialData,
    __out_opt ID3D11Buffer **ppBuffer
) {
    return m_device->CreateBuffer( pDesc, pInitialData, ppBuffer );
}

//-----------------------------------------------------------------------------

HRESULT STDMETHODCALLTYPE ID3D11DeviceProxy::CreateTexture1D(
    __in const D3D11_TEXTURE1D_DESC *pDesc,
    __in_xcount_opt(pDesc->MipLevels * pDesc->ArraySize) const D3D11_SUBRESOURCE_DATA *pInitialData,
    __out_opt ID3D11Texture1D **ppTexture1D
) {
    return m_device->CreateTexture1D( pDesc, pInitialData, ppTexture1D );
}

//-----------------------------------------------------------------------------

HRESULT STDMETHODCALLTYPE ID3D11DeviceProxy::CreateTexture2D(
    __in const D3D11_TEXTURE2D_DESC *pDesc,
    __in_xcount_opt(pDesc->MipLevels * pDesc->ArraySize) const D3D11_SUBRESOURCE_DATA *pInitialData,
    __out_opt ID3D11Texture2D **ppTexture2D
) {
    return m_device->CreateTexture2D( pDesc, pInitialData, ppTexture2D );
}

//-----------------------------------------------------------------------------

HRESULT STDMETHODCALLTYPE ID3D11DeviceProxy::CreateTexture3D(
    __in const D3D11_TEXTURE3D_DESC *pDesc,
    __in_xcount_opt(pDesc->MipLevels) const D3D11_SUBRESOURCE_DATA *pInitialData,
    __out_opt ID3D11Texture3D **ppTexture3D
) {
    return m_device->CreateTexture3D( pDesc, pInitialData, ppTexture3D );
}

//-----------------------------------------------------------------------------

HRESULT STDMETHODCALLTYPE ID3D11DeviceProxy::CreateShaderResourceView(
    __in ID3D11Resource *pResource,
    __in_opt const D3D11_SHADER_RESOURCE_VIEW_DESC *pDesc,
    __out_opt ID3D11ShaderResourceView **ppSRView
) {
    return m_device->CreateShaderResourceView( pResource, pDesc, ppSRView );
}

//-----------------------------------------------------------------------------

HRESULT STDMETHODCALLTYPE ID3D11DeviceProxy::CreateUnorderedAccessView(
    __in ID3D11Resource *pResource,
    __in_opt const D3D11_UNORDERED_ACCESS_VIEW_DESC *pDesc,
    __out_opt ID3D11UnorderedAccessView **ppUAView
) {
    return m_device->CreateUnorderedAccessView( pResource, pDesc, ppUAView );
}

//-----------------------------------------------------------------------------

HRESULT STDMETHODCALLTYPE ID3D11DeviceProxy::CreateRenderTargetView(
    __in ID3D11Resource *pResource,
    __in_opt const D3D11_RENDER_TARGET_VIEW_DESC *pDesc,
    __out_opt ID3D11RenderTargetView **ppRTView
) {
    return m_device->CreateRenderTargetView( pResource, pDesc, ppRTView );
}

//-----------------------------------------------------------------------------

HRESULT STDMETHODCALLTYPE ID3D11DeviceProxy::CreateDepthStencilView(
    __in ID3D11Resource *pResource,
    __in_opt const D3D11_DEPTH_STENCIL_VIEW_DESC *pDesc,
    __out_opt ID3D11DepthStencilView **ppDepthStencilView
) {
    return m_device->CreateDepthStencilView(
        pResource, pDesc, ppDepthStencilView
    );
}

//-----------------------------------------------------------------------------

HRESULT STDMETHODCALLTYPE ID3D11DeviceProxy::CreateInputLayout(
    __in_ecount(NumElements) const D3D11_INPUT_ELEMENT_DESC *pInputElementDescs,
    __in_range(0, D3D11_IA_VERTEX_INPUT_STRUCTURE_ELEMENT_COUNT ) UINT NumElements,
    __in const void *pShaderBytecodeWithInputSignature,
    __in SIZE_T BytecodeLength,
    __out_opt ID3D11InputLayout **ppInputLayout
) {
    return m_device->CreateInputLayout(
        pInputElementDescs,
        NumElements,
        pShaderBytecodeWithInputSignature,
        BytecodeLength,
        ppInputLayout
    );
}

//-----------------------------------------------------------------------------

HRESULT STDMETHODCALLTYPE ID3D11DeviceProxy::CreateVertexShader(
    __in const void *pShaderBytecode,
    __in SIZE_T BytecodeLength,
    __in_opt ID3D11ClassLinkage *pClassLinkage,
    __out_opt ID3D11VertexShader **ppVertexShader
) {
    return m_device->CreateVertexShader(
        pShaderBytecode, BytecodeLength,
        pClassLinkage,
        ppVertexShader
    );
}

//-----------------------------------------------------------------------------

HRESULT STDMETHODCALLTYPE ID3D11DeviceProxy::CreateGeometryShader(
    __in const void *pShaderBytecode,
    __in SIZE_T BytecodeLength,
    __in_opt ID3D11ClassLinkage *pClassLinkage,
    __out_opt ID3D11GeometryShader **ppGeometryShader
) {
    return m_device->CreateGeometryShader(
        pShaderBytecode, BytecodeLength,
        pClassLinkage,
        ppGeometryShader
    );
}

//-----------------------------------------------------------------------------

HRESULT STDMETHODCALLTYPE ID3D11DeviceProxy::CreateGeometryShaderWithStreamOutput(
    __in const void *pShaderBytecode,
    __in SIZE_T BytecodeLength,
    __in_ecount_opt(NumEntries) const D3D11_SO_DECLARATION_ENTRY *pSODeclaration,
    __in_range(0, D3D11_SO_STREAM_COUNT * D3D11_SO_OUTPUT_COMPONENT_COUNT ) UINT NumEntries,
    __in_ecount_opt(NumStrides) const UINT *pBufferStrides,
    __in_range(0, D3D11_SO_BUFFER_SLOT_COUNT ) UINT NumStrides,
    __in UINT RasterizedStream,
    __in_opt ID3D11ClassLinkage *pClassLinkage,
    __out_opt ID3D11GeometryShader **ppGeometryShader
) {
    return m_device->CreateGeometryShaderWithStreamOutput(
        pShaderBytecode,
        BytecodeLength,
        pSODeclaration,
        NumEntries,
        pBufferStrides,
        NumStrides,
        RasterizedStream,
        pClassLinkage,
        ppGeometryShader
    );
}

//-----------------------------------------------------------------------------

HRESULT STDMETHODCALLTYPE ID3D11DeviceProxy::CreatePixelShader(
    __in const void *pShaderBytecode,
    __in SIZE_T BytecodeLength,
    __in_opt ID3D11ClassLinkage *pClassLinkage,
    __out_opt ID3D11PixelShader **ppPixelShader
) {
    return m_device->CreatePixelShader(
        pShaderBytecode,
        BytecodeLength,
        pClassLinkage,
        ppPixelShader
    );
}

//-----------------------------------------------------------------------------

HRESULT STDMETHODCALLTYPE ID3D11DeviceProxy::CreateHullShader(
    __in const void *pShaderBytecode,
    __in SIZE_T BytecodeLength,
    __in_opt ID3D11ClassLinkage *pClassLinkage,
    __out_opt ID3D11HullShader **ppHullShader
) {
    return m_device->CreateHullShader(
        pShaderBytecode,
        BytecodeLength,
        pClassLinkage,
        ppHullShader
    );
}

//-----------------------------------------------------------------------------

HRESULT STDMETHODCALLTYPE ID3D11DeviceProxy::CreateDomainShader(
    __in const void *pShaderBytecode,
    __in SIZE_T BytecodeLength,
    __in_opt ID3D11ClassLinkage *pClassLinkage,
    __out_opt ID3D11DomainShader **ppDomainShader
) {
    return m_device->CreateDomainShader(
        pShaderBytecode,
        BytecodeLength,
        pClassLinkage,
        ppDomainShader
    );
}

//-----------------------------------------------------------------------------

HRESULT STDMETHODCALLTYPE ID3D11DeviceProxy::CreateComputeShader(
    __in const void *pShaderBytecode,
    __in SIZE_T BytecodeLength,
    __in_opt ID3D11ClassLinkage *pClassLinkage,
    __out_opt ID3D11ComputeShader **ppComputeShader
) {
    return m_device->CreateComputeShader(
        pShaderBytecode,
        BytecodeLength,
        pClassLinkage,
        ppComputeShader
    );
}

//-----------------------------------------------------------------------------

HRESULT STDMETHODCALLTYPE ID3D11DeviceProxy::CreateClassLinkage(
    __out ID3D11ClassLinkage **ppLinkage
) {
    return m_device->CreateClassLinkage( ppLinkage );
}

//-----------------------------------------------------------------------------

HRESULT STDMETHODCALLTYPE ID3D11DeviceProxy::CreateBlendState(
    __in const D3D11_BLEND_DESC *pBlendStateDesc,
    __out_opt ID3D11BlendState **ppBlendState
) {
    return m_device->CreateBlendState(
        pBlendStateDesc,
        ppBlendState
    );
}

//-----------------------------------------------------------------------------

HRESULT STDMETHODCALLTYPE ID3D11DeviceProxy::CreateDepthStencilState(
    __in const D3D11_DEPTH_STENCIL_DESC *pDepthStencilDesc,
    __out_opt ID3D11DepthStencilState **ppDepthStencilState
) {
    return m_device->CreateDepthStencilState(
        pDepthStencilDesc,
        ppDepthStencilState
    );
}

//-----------------------------------------------------------------------------

HRESULT STDMETHODCALLTYPE ID3D11DeviceProxy::CreateRasterizerState(
    __in const D3D11_RASTERIZER_DESC *pRasterizerDesc,
    __out_opt ID3D11RasterizerState **ppRasterizerState
) {
    return m_device->CreateRasterizerState(
        pRasterizerDesc,
        ppRasterizerState
    );
}

//-----------------------------------------------------------------------------

HRESULT STDMETHODCALLTYPE ID3D11DeviceProxy::CreateSamplerState(
    __in const D3D11_SAMPLER_DESC *pSamplerDesc,
    __out_opt ID3D11SamplerState **ppSamplerState
) {
    return m_device->CreateSamplerState( pSamplerDesc, ppSamplerState );
}

//-----------------------------------------------------------------------------

HRESULT STDMETHODCALLTYPE ID3D11DeviceProxy::CreateQuery(
    __in const D3D11_QUERY_DESC *pQueryDesc,
    __out_opt ID3D11Query **ppQuery
) {
    return m_device->CreateQuery( pQueryDesc, ppQuery );
}

//-----------------------------------------------------------------------------

HRESULT STDMETHODCALLTYPE ID3D11DeviceProxy::CreatePredicate(
    __in const D3D11_QUERY_DESC *pPredicateDesc,
    __out_opt ID3D11Predicate **ppPredicate
) {
    return m_device->CreatePredicate( pPredicateDesc, ppPredicate );
}

//-----------------------------------------------------------------------------

HRESULT STDMETHODCALLTYPE ID3D11DeviceProxy::CreateCounter(
    __in const D3D11_COUNTER_DESC *pCounterDesc,
    __out_opt ID3D11Counter **ppCounter
) {
    return m_device->CreateCounter( pCounterDesc, ppCounter );
}

//-----------------------------------------------------------------------------

HRESULT STDMETHODCALLTYPE ID3D11DeviceProxy::CreateDeferredContext(
    UINT ContextFlags,
    __out_opt ID3D11DeviceContext **ppDeferredContext
) {
    return m_device->CreateDeferredContext(
        ContextFlags,
        ppDeferredContext
    );
}

//-----------------------------------------------------------------------------

HRESULT STDMETHODCALLTYPE ID3D11DeviceProxy::OpenSharedResource(
    __in HANDLE hResource,
    __in REFIID ReturnedInterface,
    __out_opt void **ppResource
) {
    return m_device->OpenSharedResource(
        hResource,
        ReturnedInterface,
        ppResource
    );
}

//-----------------------------------------------------------------------------

HRESULT STDMETHODCALLTYPE ID3D11DeviceProxy::CheckFormatSupport(
    __in DXGI_FORMAT Format,
    __out UINT *pFormatSupport
) {
    return m_device->CheckFormatSupport( Format, pFormatSupport );
}

//-----------------------------------------------------------------------------

HRESULT STDMETHODCALLTYPE ID3D11DeviceProxy::CheckMultisampleQualityLevels(
    __in DXGI_FORMAT Format,
    __in UINT SampleCount,
    __out UINT *pNumQualityLevels
) {
    return m_device->CheckMultisampleQualityLevels(
        Format,
        SampleCount,
        pNumQualityLevels
    );
}

//-----------------------------------------------------------------------------

void STDMETHODCALLTYPE ID3D11DeviceProxy::CheckCounterInfo(
    __out D3D11_COUNTER_INFO *pCounterInfo
) {
    return m_device->CheckCounterInfo( pCounterInfo );
}

//-----------------------------------------------------------------------------

HRESULT STDMETHODCALLTYPE ID3D11DeviceProxy::CheckCounter(
    __in const D3D11_COUNTER_DESC *pDesc,
    __out D3D11_COUNTER_TYPE *pType,
    __out UINT *pActiveCounters,
    __out_ecount_opt(*pNameLength) LPSTR szName,
    __inout_opt UINT *pNameLength,
    __out_ecount_opt(*pUnitsLength) LPSTR szUnits,
    __inout_opt UINT *pUnitsLength,
    __out_ecount_opt(*pDescriptionLength) LPSTR szDescription,
    __inout_opt UINT *pDescriptionLength
) {
    return m_device->CheckCounter(
        pDesc,
        pType,
        pActiveCounters,
        szName,
        pNameLength,
        szUnits,
        pUnitsLength,
        szDescription,
        pDescriptionLength
    );
}

//-----------------------------------------------------------------------------

HRESULT STDMETHODCALLTYPE ID3D11DeviceProxy::CheckFeatureSupport(
    D3D11_FEATURE Feature,
    __out_bcount(FeatureSupportDataSize) void *pFeatureSupportData,
    UINT FeatureSupportDataSize
) {
    return m_device->CheckFeatureSupport(
        Feature,
        pFeatureSupportData, FeatureSupportDataSize
    );
}

//-----------------------------------------------------------------------------

HRESULT STDMETHODCALLTYPE ID3D11DeviceProxy::GetPrivateData(
    __in REFGUID guid,
    __inout UINT *pDataSize,
    __out_bcount_opt(*pDataSize) void *pData
) {
    return m_device->GetPrivateData( guid, pDataSize, pData );
}

//-----------------------------------------------------------------------------

HRESULT STDMETHODCALLTYPE ID3D11DeviceProxy::SetPrivateData(
    __in REFGUID guid,
    __in UINT DataSize,
    __in_bcount_opt(DataSize) const void *pData
) {
    return m_device->SetPrivateData( guid, DataSize, pData );
}

//-----------------------------------------------------------------------------

HRESULT STDMETHODCALLTYPE ID3D11DeviceProxy::SetPrivateDataInterface(
    __in REFGUID guid,
    __in_opt const IUnknown *pData
) {
    return m_device->SetPrivateDataInterface( guid, pData );
}

//-----------------------------------------------------------------------------

D3D_FEATURE_LEVEL STDMETHODCALLTYPE ID3D11DeviceProxy::GetFeatureLevel()
{
    return m_device->GetFeatureLevel();
}

//-----------------------------------------------------------------------------

UINT STDMETHODCALLTYPE ID3D11DeviceProxy::GetCreationFlags()
{
    return m_device->GetCreationFlags();
}

//-----------------------------------------------------------------------------

HRESULT STDMETHODCALLTYPE ID3D11DeviceProxy::GetDeviceRemovedReason()
{
    return m_device->GetDeviceRemovedReason();
}

//-----------------------------------------------------------------------------

void STDMETHODCALLTYPE ID3D11DeviceProxy::GetImmediateContext(
    __out ID3D11DeviceContext **ppImmediateContext
) {
    return m_device->GetImmediateContext( ppImmediateContext );
}

//-----------------------------------------------------------------------------

HRESULT STDMETHODCALLTYPE ID3D11DeviceProxy::SetExceptionMode(
    UINT RaiseFlags
) {
    return m_device->SetExceptionMode( RaiseFlags );
}

//-----------------------------------------------------------------------------

UINT STDMETHODCALLTYPE ID3D11DeviceProxy::GetExceptionMode()
{
    return m_device->GetExceptionMode();
}

//-----------------------------------------------------------------------------

#endif//SUPPORT_D3D11