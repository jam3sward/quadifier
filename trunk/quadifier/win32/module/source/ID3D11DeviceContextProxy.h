#if !defined(hive_ID3D11DeviceContextProxy_h) && (defined(SUPPORT_D3D11))
#define hive_ID3D11DeviceContextProxy_h

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
//  1. The origin of this software must not be misrepresented; you must not
//  claim that you wrote the original software.
//
//  2. If you use this software in a product, an acknowledgment in the
//  product documentation is required.
//
//  3. Altered source versions must be plainly marked as such, and must not
//  be misrepresented as being the original software.
//
//  4. This notice may not be removed or altered from any source
//  distribution.
//
//-----------------------------------------------------------------------------

#include <d3d11.h>

//-----------------------------------------------------------------------------

struct ID3D11DeviceContextProxy: public ID3D11DeviceContext {
public:
    ID3D11DeviceContextProxy( ID3D11DeviceContext *context );

    virtual ~ID3D11DeviceContextProxy();

    //--- IUnknown methods ----------------------------------------------------
    
    STDMETHOD(QueryInterface)(THIS_ REFIID riid, void** ppvObj);

    STDMETHOD_(ULONG,AddRef)(THIS);

    STDMETHOD_(ULONG,Release)(THIS);

    //--- ID3D11DeviceContext methods -----------------------------------------

    virtual void STDMETHODCALLTYPE VSSetConstantBuffers(
        __in_range( 0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT - 1 ) UINT StartSlot,
        __in_range( 0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT - StartSlot ) UINT NumBuffers,
        __in_ecount(NumBuffers) ID3D11Buffer *const *ppConstantBuffers
    );

    virtual void STDMETHODCALLTYPE PSSetShaderResources(
        __in_range( 0, D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT - 1 ) UINT StartSlot,
        __in_range( 0, D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT - StartSlot ) UINT NumViews,
        __in_ecount(NumViews) ID3D11ShaderResourceView *const *ppShaderResourceViews
    );

    virtual void STDMETHODCALLTYPE PSSetShader(
        __in_opt ID3D11PixelShader *pPixelShader,
        __in_ecount_opt(NumClassInstances) ID3D11ClassInstance *const *ppClassInstances,
        UINT NumClassInstances
    );

    virtual void STDMETHODCALLTYPE PSSetSamplers(
        __in_range( 0, D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT - 1 ) UINT StartSlot,
        __in_range( 0, D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT - StartSlot ) UINT NumSamplers,
        __in_ecount(NumSamplers) ID3D11SamplerState *const *ppSamplers
    );

    virtual void STDMETHODCALLTYPE VSSetShader(
        __in_opt ID3D11VertexShader *pVertexShader,
        __in_ecount_opt(NumClassInstances) ID3D11ClassInstance *const *ppClassInstances,
        UINT NumClassInstances
    );

    virtual void STDMETHODCALLTYPE DrawIndexed(
        __in UINT IndexCount,
        __in UINT StartIndexLocation,
        __in INT BaseVertexLocation
    );

    virtual void STDMETHODCALLTYPE Draw(
        __in UINT VertexCount,
        __in UINT StartVertexLocation
    );

    virtual HRESULT STDMETHODCALLTYPE Map(
        __in ID3D11Resource *pResource,
        __in UINT Subresource,
        __in D3D11_MAP MapType,
        __in UINT MapFlags,
        __out D3D11_MAPPED_SUBRESOURCE *pMappedResource
    );

    virtual void STDMETHODCALLTYPE Unmap(
        __in ID3D11Resource *pResource,
        __in UINT Subresource
    );

    virtual void STDMETHODCALLTYPE PSSetConstantBuffers(
        __in_range( 0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT - 1 ) UINT StartSlot,
        __in_range( 0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT - StartSlot ) UINT NumBuffers,
        __in_ecount(NumBuffers) ID3D11Buffer *const *ppConstantBuffers
    );

    virtual void STDMETHODCALLTYPE IASetInputLayout(
        __in_opt ID3D11InputLayout *pInputLayout
    );

    virtual void STDMETHODCALLTYPE IASetVertexBuffers(
        __in_range( 0, D3D11_IA_VERTEX_INPUT_RESOURCE_SLOT_COUNT - 1 ) UINT StartSlot,
        __in_range( 0, D3D11_IA_VERTEX_INPUT_RESOURCE_SLOT_COUNT - StartSlot ) UINT NumBuffers,
        __in_ecount(NumBuffers) ID3D11Buffer *const *ppVertexBuffers,
        __in_ecount(NumBuffers) const UINT *pStrides,
        __in_ecount(NumBuffers) const UINT *pOffsets
    );

    virtual void STDMETHODCALLTYPE IASetIndexBuffer(
        __in_opt ID3D11Buffer *pIndexBuffer,
        __in DXGI_FORMAT Format,
        __in UINT Offset
    );

    virtual void STDMETHODCALLTYPE DrawIndexedInstanced(
        __in UINT IndexCountPerInstance,
        __in UINT InstanceCount,
        __in UINT StartIndexLocation,
        __in INT BaseVertexLocation,
        __in UINT StartInstanceLocation
    );

    virtual void STDMETHODCALLTYPE DrawInstanced(
        __in UINT VertexCountPerInstance,
        __in UINT InstanceCount,
        __in UINT StartVertexLocation,
        __in UINT StartInstanceLocation
    );

    virtual void STDMETHODCALLTYPE GSSetConstantBuffers(
        __in_range( 0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT - 1 ) UINT StartSlot,
        __in_range( 0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT - StartSlot ) UINT NumBuffers,
        __in_ecount(NumBuffers) ID3D11Buffer *const *ppConstantBuffers
    );

    virtual void STDMETHODCALLTYPE GSSetShader(
        __in_opt ID3D11GeometryShader *pShader,
        __in_ecount_opt(NumClassInstances) ID3D11ClassInstance *const *ppClassInstances,
        UINT NumClassInstances
    );

    virtual void STDMETHODCALLTYPE IASetPrimitiveTopology(
        __in D3D11_PRIMITIVE_TOPOLOGY Topology
    );

    virtual void STDMETHODCALLTYPE VSSetShaderResources(
        __in_range( 0, D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT - 1 ) UINT StartSlot,
        __in_range( 0, D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT - StartSlot ) UINT NumViews,
        __in_ecount(NumViews) ID3D11ShaderResourceView *const *ppShaderResourceViews
    );

    virtual void STDMETHODCALLTYPE VSSetSamplers(
        __in_range( 0, D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT - 1 ) UINT StartSlot,
        __in_range( 0, D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT - StartSlot ) UINT NumSamplers,
        __in_ecount(NumSamplers) ID3D11SamplerState *const *ppSamplers
    );

    virtual void STDMETHODCALLTYPE Begin(
        __in ID3D11Asynchronous *pAsync
    );

    virtual void STDMETHODCALLTYPE End(
        __in ID3D11Asynchronous *pAsync
    );

    virtual HRESULT STDMETHODCALLTYPE GetData(
        __in ID3D11Asynchronous *pAsync,
        __out_bcount_opt( DataSize ) void *pData,
        __in UINT DataSize,
        __in UINT GetDataFlags
    );

    virtual void STDMETHODCALLTYPE SetPredication(
        __in_opt ID3D11Predicate *pPredicate,
        __in BOOL PredicateValue
    );

    virtual void STDMETHODCALLTYPE GSSetShaderResources(
        __in_range( 0, D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT - 1 ) UINT StartSlot,
        __in_range( 0, D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT - StartSlot ) UINT NumViews,
        __in_ecount(NumViews) ID3D11ShaderResourceView *const *ppShaderResourceViews
    );

    virtual void STDMETHODCALLTYPE GSSetSamplers(
        __in_range( 0, D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT - 1 ) UINT StartSlot,
        __in_range( 0, D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT - StartSlot ) UINT NumSamplers,
        __in_ecount(NumSamplers) ID3D11SamplerState *const *ppSamplers
    );

    virtual void STDMETHODCALLTYPE OMSetRenderTargets(
        __in_range( 0, D3D11_SIMULTANEOUS_RENDER_TARGET_COUNT ) UINT NumViews,
        __in_ecount_opt(NumViews) ID3D11RenderTargetView *const *ppRenderTargetViews,
        __in_opt ID3D11DepthStencilView *pDepthStencilView
    );

    virtual void STDMETHODCALLTYPE OMSetRenderTargetsAndUnorderedAccessViews(
        __in UINT NumRTVs,
        __in_ecount_opt(NumRTVs) ID3D11RenderTargetView *const *ppRenderTargetViews,
        __in_opt ID3D11DepthStencilView *pDepthStencilView,
        __in_range( 0, D3D11_PS_CS_UAV_REGISTER_COUNT - 1 ) UINT UAVStartSlot,
        __in UINT NumUAVs,
        __in_ecount_opt(NumUAVs) ID3D11UnorderedAccessView *const *ppUnorderedAccessViews,
        __in_ecount_opt(NumUAVs) const UINT *pUAVInitialCounts
    );

    virtual void STDMETHODCALLTYPE OMSetBlendState(
        __in_opt ID3D11BlendState *pBlendState,
        __in_opt const FLOAT BlendFactor[ 4 ],
        __in UINT SampleMask
    );

    virtual void STDMETHODCALLTYPE OMSetDepthStencilState(
        __in_opt ID3D11DepthStencilState *pDepthStencilState,
        __in UINT StencilRef
    );

    virtual void STDMETHODCALLTYPE SOSetTargets(
        __in_range( 0, D3D11_SO_BUFFER_SLOT_COUNT) UINT NumBuffers,
        __in_ecount_opt(NumBuffers) ID3D11Buffer *const *ppSOTargets,
        __in_ecount_opt(NumBuffers) const UINT *pOffsets
    );

    virtual void STDMETHODCALLTYPE DrawAuto();

    virtual void STDMETHODCALLTYPE DrawIndexedInstancedIndirect(
        __in ID3D11Buffer *pBufferForArgs,
        __in UINT AlignedByteOffsetForArgs
    );

    virtual void STDMETHODCALLTYPE DrawInstancedIndirect(
        __in ID3D11Buffer *pBufferForArgs,
        __in UINT AlignedByteOffsetForArgs
    );

    virtual void STDMETHODCALLTYPE Dispatch(
        __in UINT ThreadGroupCountX,
        __in UINT ThreadGroupCountY,
        __in UINT ThreadGroupCountZ
    );

    virtual void STDMETHODCALLTYPE DispatchIndirect(
        __in ID3D11Buffer *pBufferForArgs,
        __in UINT AlignedByteOffsetForArgs
    );

    virtual void STDMETHODCALLTYPE RSSetState(
        __in_opt ID3D11RasterizerState *pRasterizerState
    );

    virtual void STDMETHODCALLTYPE RSSetViewports(
        __in_range(0, D3D11_VIEWPORT_AND_SCISSORRECT_OBJECT_COUNT_PER_PIPELINE) UINT NumViewports,
        __in_ecount_opt(NumViewports) const D3D11_VIEWPORT *pViewports
    );

    virtual void STDMETHODCALLTYPE RSSetScissorRects(
        __in_range(0, D3D11_VIEWPORT_AND_SCISSORRECT_OBJECT_COUNT_PER_PIPELINE) UINT NumRects,
        __in_ecount_opt(NumRects) const D3D11_RECT *pRects
    );

    virtual void STDMETHODCALLTYPE CopySubresourceRegion(
        __in ID3D11Resource *pDstResource,
        __in UINT DstSubresource,
        __in UINT DstX,
        __in UINT DstY,
        __in UINT DstZ,
        __in ID3D11Resource *pSrcResource,
        __in UINT SrcSubresource,
        __in_opt const D3D11_BOX *pSrcBox
    );

    virtual void STDMETHODCALLTYPE CopyResource(
        __in ID3D11Resource *pDstResource,
        __in ID3D11Resource *pSrcResource
    );

    virtual void STDMETHODCALLTYPE UpdateSubresource(
        __in ID3D11Resource *pDstResource,
        __in UINT DstSubresource,
        __in_opt const D3D11_BOX *pDstBox,
        __in const void *pSrcData,
        __in UINT SrcRowPitch,
        __in UINT SrcDepthPitch
    );

    virtual void STDMETHODCALLTYPE CopyStructureCount(
        __in ID3D11Buffer *pDstBuffer,
        __in UINT DstAlignedByteOffset,
        __in ID3D11UnorderedAccessView *pSrcView
    );

    virtual void STDMETHODCALLTYPE ClearRenderTargetView(
        __in ID3D11RenderTargetView *pRenderTargetView,
        __in const FLOAT ColorRGBA[ 4 ]
    );

    virtual void STDMETHODCALLTYPE ClearUnorderedAccessViewUint(
        __in ID3D11UnorderedAccessView *pUnorderedAccessView,
        __in const UINT Values[ 4 ]
    );

    virtual void STDMETHODCALLTYPE ClearUnorderedAccessViewFloat(
        __in ID3D11UnorderedAccessView *pUnorderedAccessView,
        __in const FLOAT Values[ 4 ]
    );

    virtual void STDMETHODCALLTYPE ClearDepthStencilView(
        __in ID3D11DepthStencilView *pDepthStencilView,
        __in UINT ClearFlags,
        __in FLOAT Depth,
        __in UINT8 Stencil
    );

    virtual void STDMETHODCALLTYPE GenerateMips(
        __in ID3D11ShaderResourceView *pShaderResourceView
    );

    virtual void STDMETHODCALLTYPE SetResourceMinLOD(
        __in ID3D11Resource *pResource,
        FLOAT MinLOD
    );

    virtual FLOAT STDMETHODCALLTYPE GetResourceMinLOD(
        __in ID3D11Resource *pResource
    );

    virtual void STDMETHODCALLTYPE ResolveSubresource(
        __in ID3D11Resource *pDstResource,
        __in UINT DstSubresource,
        __in ID3D11Resource *pSrcResource,
        __in UINT SrcSubresource,
        __in DXGI_FORMAT Format
    );

    virtual void STDMETHODCALLTYPE ExecuteCommandList(
        __in ID3D11CommandList *pCommandList,
        BOOL RestoreContextState
    );

    virtual void STDMETHODCALLTYPE HSSetShaderResources(
        __in_range( 0, D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT - 1 ) UINT StartSlot,
        __in_range( 0, D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT - StartSlot ) UINT NumViews,
        __in_ecount(NumViews) ID3D11ShaderResourceView *const *ppShaderResourceViews
    );

    virtual void STDMETHODCALLTYPE HSSetShader(
        __in_opt ID3D11HullShader *pHullShader,
        __in_ecount_opt(NumClassInstances) ID3D11ClassInstance *const *ppClassInstances,
        UINT NumClassInstances
    );

    virtual void STDMETHODCALLTYPE HSSetSamplers(
        __in_range( 0, D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT - 1 ) UINT StartSlot,
        __in_range( 0, D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT - StartSlot ) UINT NumSamplers,
        __in_ecount(NumSamplers) ID3D11SamplerState *const *ppSamplers
    );

    virtual void STDMETHODCALLTYPE HSSetConstantBuffers(
        __in_range( 0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT - 1 ) UINT StartSlot,
        __in_range( 0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT - StartSlot ) UINT NumBuffers,
        __in_ecount(NumBuffers) ID3D11Buffer *const *ppConstantBuffers
    );

    virtual void STDMETHODCALLTYPE DSSetShaderResources(
        __in_range( 0, D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT - 1 ) UINT StartSlot,
        __in_range( 0, D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT - StartSlot ) UINT NumViews,
        __in_ecount(NumViews) ID3D11ShaderResourceView *const *ppShaderResourceViews
    );

    virtual void STDMETHODCALLTYPE DSSetShader(
        __in_opt ID3D11DomainShader *pDomainShader,
        __in_ecount_opt(NumClassInstances) ID3D11ClassInstance *const *ppClassInstances,
        UINT NumClassInstances
    );

    virtual void STDMETHODCALLTYPE DSSetSamplers(
        __in_range( 0, D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT - 1 ) UINT StartSlot,
        __in_range( 0, D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT - StartSlot ) UINT NumSamplers,
        __in_ecount(NumSamplers) ID3D11SamplerState *const *ppSamplers
    );

    virtual void STDMETHODCALLTYPE DSSetConstantBuffers(
        __in_range( 0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT - 1 ) UINT StartSlot,
        __in_range( 0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT - StartSlot ) UINT NumBuffers,
        __in_ecount(NumBuffers) ID3D11Buffer *const *ppConstantBuffers
    );

    virtual void STDMETHODCALLTYPE CSSetShaderResources(
        __in_range( 0, D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT - 1 ) UINT StartSlot,
        __in_range( 0, D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT - StartSlot ) UINT NumViews,
        __in_ecount(NumViews) ID3D11ShaderResourceView *const *ppShaderResourceViews
    );

    virtual void STDMETHODCALLTYPE CSSetUnorderedAccessViews(
        __in_range( 0, D3D11_PS_CS_UAV_REGISTER_COUNT - 1 ) UINT StartSlot,
        __in_range( 0, D3D11_PS_CS_UAV_REGISTER_COUNT - StartSlot ) UINT NumUAVs,
        __in_ecount(NumUAVs) ID3D11UnorderedAccessView *const *ppUnorderedAccessViews,
        __in_ecount(NumUAVs) const UINT *pUAVInitialCounts
    );

    virtual void STDMETHODCALLTYPE CSSetShader(
        __in_opt ID3D11ComputeShader *pComputeShader,
        __in_ecount_opt(NumClassInstances) ID3D11ClassInstance *const *ppClassInstances,
        UINT NumClassInstances
    );

    virtual void STDMETHODCALLTYPE CSSetSamplers(
        __in_range( 0, D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT - 1 ) UINT StartSlot,
        __in_range( 0, D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT - StartSlot ) UINT NumSamplers,
        __in_ecount(NumSamplers) ID3D11SamplerState *const *ppSamplers
    );

    virtual void STDMETHODCALLTYPE CSSetConstantBuffers(
        __in_range( 0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT - 1 ) UINT StartSlot,
        __in_range( 0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT - StartSlot ) UINT NumBuffers,
        __in_ecount(NumBuffers) ID3D11Buffer *const *ppConstantBuffers
    );

    virtual void STDMETHODCALLTYPE VSGetConstantBuffers(
        __in_range( 0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT - 1 ) UINT StartSlot,
        __in_range( 0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT - StartSlot ) UINT NumBuffers,
        __out_ecount(NumBuffers) ID3D11Buffer **ppConstantBuffers
    );

    virtual void STDMETHODCALLTYPE PSGetShaderResources(
        __in_range( 0, D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT - 1 ) UINT StartSlot,
        __in_range( 0, D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT - StartSlot ) UINT NumViews,
        __out_ecount(NumViews) ID3D11ShaderResourceView **ppShaderResourceViews
    );

    virtual void STDMETHODCALLTYPE PSGetShader(
        __out_opt ID3D11PixelShader **ppPixelShader,
        __out_ecount_opt(*pNumClassInstances) ID3D11ClassInstance **ppClassInstances,
        __inout_opt UINT *pNumClassInstances
    );

    virtual void STDMETHODCALLTYPE PSGetSamplers(
        __in_range( 0, D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT - 1 ) UINT StartSlot,
        __in_range( 0, D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT - StartSlot ) UINT NumSamplers,
        __out_ecount(NumSamplers) ID3D11SamplerState **ppSamplers
    );

    virtual void STDMETHODCALLTYPE VSGetShader(
        __out_opt ID3D11VertexShader **ppVertexShader,
        __out_ecount_opt(*pNumClassInstances) ID3D11ClassInstance **ppClassInstances,
        __inout_opt UINT *pNumClassInstances
    );

    virtual void STDMETHODCALLTYPE PSGetConstantBuffers(
        __in_range( 0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT - 1 ) UINT StartSlot,
        __in_range( 0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT - StartSlot ) UINT NumBuffers,
        __out_ecount(NumBuffers) ID3D11Buffer **ppConstantBuffers
    );

    virtual void STDMETHODCALLTYPE IAGetInputLayout(
        __out ID3D11InputLayout **ppInputLayout
    );

    virtual void STDMETHODCALLTYPE IAGetVertexBuffers(
        __in_range( 0, D3D11_IA_VERTEX_INPUT_RESOURCE_SLOT_COUNT - 1 ) UINT StartSlot,
        __in_range( 0, D3D11_IA_VERTEX_INPUT_RESOURCE_SLOT_COUNT - StartSlot ) UINT NumBuffers,
        __out_ecount_opt(NumBuffers) ID3D11Buffer **ppVertexBuffers,
        __out_ecount_opt(NumBuffers) UINT *pStrides,
        __out_ecount_opt(NumBuffers) UINT *pOffsets
    );

    virtual void STDMETHODCALLTYPE IAGetIndexBuffer(
        __out_opt ID3D11Buffer **pIndexBuffer,
        __out_opt DXGI_FORMAT *Format,
        __out_opt UINT *Offset
    );

    virtual void STDMETHODCALLTYPE GSGetConstantBuffers(
        __in_range( 0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT - 1 ) UINT StartSlot,
        __in_range( 0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT - StartSlot ) UINT NumBuffers,
        __out_ecount(NumBuffers) ID3D11Buffer **ppConstantBuffers
    );

    virtual void STDMETHODCALLTYPE GSGetShader(
        __out_opt ID3D11GeometryShader **ppGeometryShader,
        __out_ecount_opt(*pNumClassInstances) ID3D11ClassInstance **ppClassInstances,
        __inout_opt UINT *pNumClassInstances
    );

    virtual void STDMETHODCALLTYPE IAGetPrimitiveTopology(
        __out D3D11_PRIMITIVE_TOPOLOGY *pTopology
    );

    virtual void STDMETHODCALLTYPE VSGetShaderResources(
        __in_range( 0, D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT - 1 ) UINT StartSlot,
        __in_range( 0, D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT - StartSlot ) UINT NumViews,
        __out_ecount(NumViews) ID3D11ShaderResourceView **ppShaderResourceViews
    );

    virtual void STDMETHODCALLTYPE VSGetSamplers(
        __in_range( 0, D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT - 1 ) UINT StartSlot,
        __in_range( 0, D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT - StartSlot ) UINT NumSamplers,
        __out_ecount(NumSamplers) ID3D11SamplerState **ppSamplers
    );

    virtual void STDMETHODCALLTYPE GetPredication(
        __out_opt ID3D11Predicate **ppPredicate,
        __out_opt BOOL *pPredicateValue
    );

    virtual void STDMETHODCALLTYPE GSGetShaderResources(
        __in_range( 0, D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT - 1 ) UINT StartSlot,
        __in_range( 0, D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT - StartSlot ) UINT NumViews,
        __out_ecount(NumViews) ID3D11ShaderResourceView **ppShaderResourceViews
    );

    virtual void STDMETHODCALLTYPE GSGetSamplers(
        __in_range( 0, D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT - 1 ) UINT StartSlot,
        __in_range( 0, D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT - StartSlot ) UINT NumSamplers,
        __out_ecount(NumSamplers) ID3D11SamplerState **ppSamplers
    );

    virtual void STDMETHODCALLTYPE OMGetRenderTargets(
        __in_range( 0, D3D11_SIMULTANEOUS_RENDER_TARGET_COUNT ) UINT NumViews,
        __out_ecount_opt(NumViews) ID3D11RenderTargetView **ppRenderTargetViews,
        __out_opt ID3D11DepthStencilView **ppDepthStencilView
    );

    virtual void STDMETHODCALLTYPE OMGetRenderTargetsAndUnorderedAccessViews(
        __in_range( 0, D3D11_SIMULTANEOUS_RENDER_TARGET_COUNT ) UINT NumRTVs,
        __out_ecount_opt(NumRTVs) ID3D11RenderTargetView **ppRenderTargetViews,
        __out_opt ID3D11DepthStencilView **ppDepthStencilView,
        __in_range( 0, D3D11_PS_CS_UAV_REGISTER_COUNT - 1 ) UINT UAVStartSlot,
        __in_range( 0, D3D11_PS_CS_UAV_REGISTER_COUNT - UAVStartSlot ) UINT NumUAVs,
        __out_ecount_opt(NumUAVs) ID3D11UnorderedAccessView **ppUnorderedAccessViews
    );

    virtual void STDMETHODCALLTYPE OMGetBlendState(
        __out_opt ID3D11BlendState **ppBlendState,
        __out_opt FLOAT BlendFactor[ 4 ],
        __out_opt UINT *pSampleMask
    );

    virtual void STDMETHODCALLTYPE OMGetDepthStencilState(
        __out_opt ID3D11DepthStencilState **ppDepthStencilState,
        __out_opt UINT *pStencilRef
    );

    virtual void STDMETHODCALLTYPE SOGetTargets(
        __in_range( 0, D3D11_SO_BUFFER_SLOT_COUNT ) UINT NumBuffers,
        __out_ecount(NumBuffers) ID3D11Buffer **ppSOTargets
    );

    virtual void STDMETHODCALLTYPE RSGetState(
        __out ID3D11RasterizerState **ppRasterizerState
    );

    virtual void STDMETHODCALLTYPE RSGetViewports(
        __inout /*_range(0, D3D11_VIEWPORT_AND_SCISSORRECT_OBJECT_COUNT_PER_PIPELINE )*/  UINT *pNumViewports,
        __out_ecount_opt(*pNumViewports) D3D11_VIEWPORT *pViewports
    );

    virtual void STDMETHODCALLTYPE RSGetScissorRects(
        __inout /*_range(0, D3D11_VIEWPORT_AND_SCISSORRECT_OBJECT_COUNT_PER_PIPELINE )*/  UINT *pNumRects,
        __out_ecount_opt(*pNumRects) D3D11_RECT *pRects
    );

    virtual void STDMETHODCALLTYPE HSGetShaderResources(
        __in_range( 0, D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT - 1 ) UINT StartSlot,
        __in_range( 0, D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT - StartSlot ) UINT NumViews,
        __out_ecount(NumViews) ID3D11ShaderResourceView **ppShaderResourceViews
    );

    virtual void STDMETHODCALLTYPE HSGetShader(
        __out_opt ID3D11HullShader **ppHullShader,
        __out_ecount_opt(*pNumClassInstances) ID3D11ClassInstance **ppClassInstances,
        __inout_opt UINT *pNumClassInstances
    );

    virtual void STDMETHODCALLTYPE HSGetSamplers(
        __in_range( 0, D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT - 1 ) UINT StartSlot,
        __in_range( 0, D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT - StartSlot ) UINT NumSamplers,
        __out_ecount(NumSamplers) ID3D11SamplerState **ppSamplers
    );

    virtual void STDMETHODCALLTYPE HSGetConstantBuffers(
        __in_range( 0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT - 1 ) UINT StartSlot,
        __in_range( 0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT - StartSlot ) UINT NumBuffers,
        __out_ecount(NumBuffers) ID3D11Buffer **ppConstantBuffers
    );

    virtual void STDMETHODCALLTYPE DSGetShaderResources(
        __in_range( 0, D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT - 1 ) UINT StartSlot,
        __in_range( 0, D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT - StartSlot ) UINT NumViews,
        __out_ecount(NumViews) ID3D11ShaderResourceView **ppShaderResourceViews
    );

    virtual void STDMETHODCALLTYPE DSGetShader(
        __out_opt ID3D11DomainShader **ppDomainShader,
        __out_ecount_opt(*pNumClassInstances) ID3D11ClassInstance **ppClassInstances,
        __inout_opt UINT *pNumClassInstances
    );

    virtual void STDMETHODCALLTYPE DSGetSamplers(
        __in_range( 0, D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT - 1 ) UINT StartSlot,
        __in_range( 0, D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT - StartSlot ) UINT NumSamplers,
        __out_ecount(NumSamplers) ID3D11SamplerState **ppSamplers
    );

    virtual void STDMETHODCALLTYPE DSGetConstantBuffers(
        __in_range( 0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT - 1 ) UINT StartSlot,
        __in_range( 0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT - StartSlot ) UINT NumBuffers,
        __out_ecount(NumBuffers) ID3D11Buffer **ppConstantBuffers
    );

    virtual void STDMETHODCALLTYPE CSGetShaderResources(
        __in_range( 0, D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT - 1 ) UINT StartSlot,
        __in_range( 0, D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT - StartSlot ) UINT NumViews,
        __out_ecount(NumViews) ID3D11ShaderResourceView **ppShaderResourceViews
    );

    virtual void STDMETHODCALLTYPE CSGetUnorderedAccessViews(
        __in_range( 0, D3D11_PS_CS_UAV_REGISTER_COUNT - 1 ) UINT StartSlot,
        __in_range( 0, D3D11_PS_CS_UAV_REGISTER_COUNT - StartSlot ) UINT NumUAVs,
        __out_ecount(NumUAVs) ID3D11UnorderedAccessView **ppUnorderedAccessViews
    );

    virtual void STDMETHODCALLTYPE CSGetShader(
        __out_opt ID3D11ComputeShader **ppComputeShader,
        __out_ecount_opt(*pNumClassInstances) ID3D11ClassInstance **ppClassInstances,
        __inout_opt UINT *pNumClassInstances
    );

    virtual void STDMETHODCALLTYPE CSGetSamplers(
        __in_range( 0, D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT - 1 ) UINT StartSlot,
        __in_range( 0, D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT - StartSlot ) UINT NumSamplers,
        __out_ecount(NumSamplers) ID3D11SamplerState **ppSamplers
    );

    virtual void STDMETHODCALLTYPE CSGetConstantBuffers(
        __in_range( 0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT - 1 ) UINT StartSlot,
        __in_range( 0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT - StartSlot ) UINT NumBuffers,
        __out_ecount(NumBuffers) ID3D11Buffer **ppConstantBuffers
    );

    virtual void STDMETHODCALLTYPE ClearState();

    virtual void STDMETHODCALLTYPE Flush();

    virtual D3D11_DEVICE_CONTEXT_TYPE STDMETHODCALLTYPE GetType();

    virtual UINT STDMETHODCALLTYPE GetContextFlags();

    virtual HRESULT STDMETHODCALLTYPE FinishCommandList( 
        BOOL RestoreDeferredContextState,
        __out_opt ID3D11CommandList **ppCommandList
    );

    virtual void STDMETHODCALLTYPE GetDevice(
        __out ID3D11Device **ppDevice
    );

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

private:
    ID3D11DeviceContext *m_device;
};

//-----------------------------------------------------------------------------

#endif//hive_ID3D11DeviceContextProxy_h