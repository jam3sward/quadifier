#if !defined(hive_ID3D11DeviceProxy_h) && (defined(SUPPORT_D3D11))
#define hive_ID3D11DeviceProxy_h

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
#include "DXGIDeviceProxy.h"

//-----------------------------------------------------------------------------

/**
 * Proxy class to replace the normal ID3D11Device interface.
 */
struct ID3D11DeviceProxy : public ID3D11Device {
public:
    ID3D11DeviceProxy(
        ID3D11Device *device
    );

    virtual ~ID3D11DeviceProxy();

    //--- IUnknown methods ----------------------------------------------------
    
    STDMETHOD(QueryInterface)(THIS_ REFIID riid, void** ppvObj);

    STDMETHOD_(ULONG,AddRef)(THIS);

    STDMETHOD_(ULONG,Release)(THIS);

    //--- ID3D11Device methods ------------------------------------------------

    virtual HRESULT STDMETHODCALLTYPE CreateBuffer(
        __in const D3D11_BUFFER_DESC *pDesc,
        __in_opt const D3D11_SUBRESOURCE_DATA *pInitialData,
        __out_opt ID3D11Buffer **ppBuffer
    );

    virtual HRESULT STDMETHODCALLTYPE CreateTexture1D(
        __in const D3D11_TEXTURE1D_DESC *pDesc,
        __in_xcount_opt(pDesc->MipLevels * pDesc->ArraySize) const D3D11_SUBRESOURCE_DATA *pInitialData,
        __out_opt ID3D11Texture1D **ppTexture1D
    );

    virtual HRESULT STDMETHODCALLTYPE CreateTexture2D(
        __in const D3D11_TEXTURE2D_DESC *pDesc,
        __in_xcount_opt(pDesc->MipLevels * pDesc->ArraySize) const D3D11_SUBRESOURCE_DATA *pInitialData,
        __out_opt ID3D11Texture2D **ppTexture2D
    );

    virtual HRESULT STDMETHODCALLTYPE CreateTexture3D(
        __in const D3D11_TEXTURE3D_DESC *pDesc,
        __in_xcount_opt(pDesc->MipLevels) const D3D11_SUBRESOURCE_DATA *pInitialData,
        __out_opt ID3D11Texture3D **ppTexture3D
    );

    virtual HRESULT STDMETHODCALLTYPE CreateShaderResourceView(
        __in ID3D11Resource *pResource,
        __in_opt const D3D11_SHADER_RESOURCE_VIEW_DESC *pDesc,
        __out_opt ID3D11ShaderResourceView **ppSRView
    );

    virtual HRESULT STDMETHODCALLTYPE CreateUnorderedAccessView(
        __in ID3D11Resource *pResource,
        __in_opt const D3D11_UNORDERED_ACCESS_VIEW_DESC *pDesc,
        __out_opt ID3D11UnorderedAccessView **ppUAView
    );
    
    virtual HRESULT STDMETHODCALLTYPE CreateRenderTargetView(
        __in ID3D11Resource *pResource,
        __in_opt const D3D11_RENDER_TARGET_VIEW_DESC *pDesc,
        __out_opt ID3D11RenderTargetView **ppRTView
    );

    virtual HRESULT STDMETHODCALLTYPE CreateDepthStencilView(
        __in ID3D11Resource *pResource,
        __in_opt const D3D11_DEPTH_STENCIL_VIEW_DESC *pDesc,
        __out_opt ID3D11DepthStencilView **ppDepthStencilView
    );

    virtual HRESULT STDMETHODCALLTYPE CreateInputLayout(
        __in_ecount(NumElements) const D3D11_INPUT_ELEMENT_DESC *pInputElementDescs,
        __in_range(0, D3D11_IA_VERTEX_INPUT_STRUCTURE_ELEMENT_COUNT ) UINT NumElements,
        __in const void *pShaderBytecodeWithInputSignature,
        __in SIZE_T BytecodeLength,
        __out_opt ID3D11InputLayout **ppInputLayout
    );

    virtual HRESULT STDMETHODCALLTYPE CreateVertexShader(
        __in const void *pShaderBytecode,
        __in SIZE_T BytecodeLength,
        __in_opt ID3D11ClassLinkage *pClassLinkage,
        __out_opt ID3D11VertexShader **ppVertexShader
    );

    virtual HRESULT STDMETHODCALLTYPE CreateGeometryShader(
        __in const void *pShaderBytecode,
        __in SIZE_T BytecodeLength,
        __in_opt ID3D11ClassLinkage *pClassLinkage,
        __out_opt ID3D11GeometryShader **ppGeometryShader
    );

    virtual HRESULT STDMETHODCALLTYPE CreateGeometryShaderWithStreamOutput(
        __in const void *pShaderBytecode,
        __in SIZE_T BytecodeLength,
        __in_ecount_opt(NumEntries) const D3D11_SO_DECLARATION_ENTRY *pSODeclaration,
        __in_range(0, D3D11_SO_STREAM_COUNT * D3D11_SO_OUTPUT_COMPONENT_COUNT ) UINT NumEntries,
        __in_ecount_opt(NumStrides) const UINT *pBufferStrides,
        __in_range(0, D3D11_SO_BUFFER_SLOT_COUNT ) UINT NumStrides,
        __in UINT RasterizedStream,
        __in_opt ID3D11ClassLinkage *pClassLinkage,
        __out_opt ID3D11GeometryShader **ppGeometryShader
    );

    virtual HRESULT STDMETHODCALLTYPE CreatePixelShader(
        __in const void *pShaderBytecode,
        __in SIZE_T BytecodeLength,
        __in_opt ID3D11ClassLinkage *pClassLinkage,
        __out_opt ID3D11PixelShader **ppPixelShader
    );

    virtual HRESULT STDMETHODCALLTYPE CreateHullShader(
        __in const void *pShaderBytecode,
        __in SIZE_T BytecodeLength,
        __in_opt ID3D11ClassLinkage *pClassLinkage,
        __out_opt ID3D11HullShader **ppHullShader
    );

    virtual HRESULT STDMETHODCALLTYPE CreateDomainShader(
        __in const void *pShaderBytecode,
        __in SIZE_T BytecodeLength,
        __in_opt ID3D11ClassLinkage *pClassLinkage,
        __out_opt ID3D11DomainShader **ppDomainShader
    );

    virtual HRESULT STDMETHODCALLTYPE CreateComputeShader(
        __in const void *pShaderBytecode,
        __in SIZE_T BytecodeLength,
        __in_opt ID3D11ClassLinkage *pClassLinkage,
        __out_opt ID3D11ComputeShader **ppComputeShader
    );

    virtual HRESULT STDMETHODCALLTYPE CreateClassLinkage(
        __out ID3D11ClassLinkage **ppLinkage
    );

    virtual HRESULT STDMETHODCALLTYPE CreateBlendState(
        __in const D3D11_BLEND_DESC *pBlendStateDesc,
        __out_opt ID3D11BlendState **ppBlendState
    );

    virtual HRESULT STDMETHODCALLTYPE CreateDepthStencilState(
        __in const D3D11_DEPTH_STENCIL_DESC *pDepthStencilDesc,
        __out_opt ID3D11DepthStencilState **ppDepthStencilState
    );

    virtual HRESULT STDMETHODCALLTYPE CreateRasterizerState(
        __in const D3D11_RASTERIZER_DESC *pRasterizerDesc,
        __out_opt ID3D11RasterizerState **ppRasterizerState
    );

    virtual HRESULT STDMETHODCALLTYPE CreateSamplerState(
        __in const D3D11_SAMPLER_DESC *pSamplerDesc,
        __out_opt ID3D11SamplerState **ppSamplerState
    );

    virtual HRESULT STDMETHODCALLTYPE CreateQuery(
        __in const D3D11_QUERY_DESC *pQueryDesc,
        __out_opt ID3D11Query **ppQuery
    );

    virtual HRESULT STDMETHODCALLTYPE CreatePredicate(
        __in const D3D11_QUERY_DESC *pPredicateDesc,
        __out_opt ID3D11Predicate **ppPredicate
    );

    virtual HRESULT STDMETHODCALLTYPE CreateCounter(
        __in const D3D11_COUNTER_DESC *pCounterDesc,
        __out_opt ID3D11Counter **ppCounter
    );

    virtual HRESULT STDMETHODCALLTYPE CreateDeferredContext(
        UINT ContextFlags,
        __out_opt ID3D11DeviceContext **ppDeferredContext
    );

    virtual HRESULT STDMETHODCALLTYPE OpenSharedResource(
        __in HANDLE hResource,
        __in REFIID ReturnedInterface,
        __out_opt void **ppResource
    );

    virtual HRESULT STDMETHODCALLTYPE CheckFormatSupport(
        __in DXGI_FORMAT Format,
        __out UINT *pFormatSupport
    );

    virtual HRESULT STDMETHODCALLTYPE CheckMultisampleQualityLevels(
        __in DXGI_FORMAT Format,
        __in UINT SampleCount,
        __out UINT *pNumQualityLevels
    );

    virtual void STDMETHODCALLTYPE CheckCounterInfo(
        __out D3D11_COUNTER_INFO *pCounterInfo
    );

    virtual HRESULT STDMETHODCALLTYPE CheckCounter(
        __in const D3D11_COUNTER_DESC *pDesc,
        __out D3D11_COUNTER_TYPE *pType,
        __out UINT *pActiveCounters,
        __out_ecount_opt(*pNameLength) LPSTR szName,
        __inout_opt UINT *pNameLength,
        __out_ecount_opt(*pUnitsLength) LPSTR szUnits,
        __inout_opt UINT *pUnitsLength,
        __out_ecount_opt(*pDescriptionLength) LPSTR szDescription,
        __inout_opt UINT *pDescriptionLength
    );

    virtual HRESULT STDMETHODCALLTYPE CheckFeatureSupport(
        D3D11_FEATURE Feature,
        __out_bcount(FeatureSupportDataSize) void *pFeatureSupportData,
        UINT FeatureSupportDataSize
    );

    virtual HRESULT STDMETHODCALLTYPE GetPrivateData(
        __in REFGUID guid,
        __inout UINT *pDataSize,
        __out_bcount_opt(*pDataSize) void *pData
    );

    virtual HRESULT STDMETHODCALLTYPE SetPrivateData(
        __in REFGUID guid,
        __in UINT DataSize,
        __in_bcount_opt(DataSize) const void *pData
    );

    virtual HRESULT STDMETHODCALLTYPE SetPrivateDataInterface(
        __in REFGUID guid,
        __in_opt const IUnknown *pData
    );

    virtual D3D_FEATURE_LEVEL STDMETHODCALLTYPE GetFeatureLevel();

    virtual UINT STDMETHODCALLTYPE GetCreationFlags();

    virtual HRESULT STDMETHODCALLTYPE GetDeviceRemovedReason();

    virtual void STDMETHODCALLTYPE GetImmediateContext(
    __out ID3D11DeviceContext **ppImmediateContext
        );

    virtual HRESULT STDMETHODCALLTYPE SetExceptionMode(
        UINT RaiseFlags
    );

    virtual UINT STDMETHODCALLTYPE GetExceptionMode();

private:
    ID3D11Device *m_device;
    std::shared_ptr<DXGIDeviceProxy> m_dxgiDeviceProxy;
};

//-----------------------------------------------------------------------------

#endif//hive_ID3D11DeviceProxy_h
