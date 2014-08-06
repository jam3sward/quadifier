#if defined(SUPPORT_D3D11)

#include "ID3D11DeviceContextProxy.h"
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

ID3D11DeviceContextProxy::ID3D11DeviceContextProxy(
    ID3D11DeviceContext *context
)
    : m_device( context )
{
    Log::print() << "ID3D11DeviceContextProxy(" << context << ")\n";
}

//-----------------------------------------------------------------------------

ID3D11DeviceContextProxy::~ID3D11DeviceContextProxy()
{
}

//-----------------------------------------------------------------------------

void STDMETHODCALLTYPE ID3D11DeviceContextProxy::VSSetConstantBuffers( 
    __in_range( 0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT - 1 ) UINT StartSlot,
    __in_range( 0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT - StartSlot ) UINT NumBuffers,
    __in_ecount(NumBuffers) ID3D11Buffer *const *ppConstantBuffers
) {
    m_device->VSSetConstantBuffers(
        StartSlot,
        NumBuffers,
        ppConstantBuffers
    );
}

//-----------------------------------------------------------------------------

void STDMETHODCALLTYPE ID3D11DeviceContextProxy::PSSetShaderResources( 
    __in_range( 0, D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT - 1 ) UINT StartSlot,
    __in_range( 0, D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT - StartSlot ) UINT NumViews,
    __in_ecount(NumViews) ID3D11ShaderResourceView *const *ppShaderResourceViews)
{
    m_device->PSSetShaderResources(StartSlot, NumViews, ppShaderResourceViews);
}

//-----------------------------------------------------------------------------
  
void STDMETHODCALLTYPE ID3D11DeviceContextProxy::PSSetShader( 
    __in_opt ID3D11PixelShader *pPixelShader,
    __in_ecount_opt(NumClassInstances) ID3D11ClassInstance *const *ppClassInstances,
    UINT NumClassInstances)
{
    m_device->PSSetShader(pPixelShader, ppClassInstances, NumClassInstances);
}

//-----------------------------------------------------------------------------

void STDMETHODCALLTYPE ID3D11DeviceContextProxy::PSSetSamplers( 
    __in_range( 0, D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT - 1 ) UINT StartSlot,
    __in_range( 0, D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT - StartSlot ) UINT NumSamplers,
    __in_ecount(NumSamplers) ID3D11SamplerState *const *ppSamplers)
{
    m_device->PSSetSamplers(StartSlot, NumSamplers, ppSamplers);
}
 
//-----------------------------------------------------------------------------
   
void STDMETHODCALLTYPE ID3D11DeviceContextProxy::VSSetShader( 
    __in_opt ID3D11VertexShader *pVertexShader,
    __in_ecount_opt(NumClassInstances) ID3D11ClassInstance *const *ppClassInstances,
    UINT NumClassInstances)
{
    m_device->VSSetShader(pVertexShader, ppClassInstances, NumClassInstances);
}

//-----------------------------------------------------------------------------
    
void STDMETHODCALLTYPE ID3D11DeviceContextProxy::DrawIndexed( 
    __in UINT IndexCount,
    __in UINT StartIndexLocation,
    __in INT BaseVertexLocation)
{
    m_device->DrawIndexed(IndexCount, StartIndexLocation, BaseVertexLocation);
}

//-----------------------------------------------------------------------------
 
void STDMETHODCALLTYPE ID3D11DeviceContextProxy::Draw( 
    __in UINT VertexCount,
    __in UINT StartVertexLocation)
{
    m_device->Draw(VertexCount, StartVertexLocation);
}

//-----------------------------------------------------------------------------
    
HRESULT STDMETHODCALLTYPE ID3D11DeviceContextProxy::Map( 
    __in ID3D11Resource *pResource,
    __in UINT Subresource,
    __in D3D11_MAP MapType,
    __in UINT MapFlags,
    __out D3D11_MAPPED_SUBRESOURCE *pMappedResource)
{
    return m_device->Map(pResource, Subresource, MapType, MapFlags, pMappedResource);
}

//-----------------------------------------------------------------------------
    
void STDMETHODCALLTYPE ID3D11DeviceContextProxy::Unmap( 
    __in ID3D11Resource *pResource,
    __in UINT Subresource)
{
    m_device->Unmap(pResource, Subresource);
}

//-----------------------------------------------------------------------------
    
void STDMETHODCALLTYPE ID3D11DeviceContextProxy::PSSetConstantBuffers( 
    __in_range( 0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT - 1 ) UINT StartSlot,
    __in_range( 0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT - StartSlot ) UINT NumBuffers,
    __in_ecount(NumBuffers) ID3D11Buffer *const *ppConstantBuffers)
{
    m_device->PSSetConstantBuffers(StartSlot, NumBuffers, ppConstantBuffers);
}

//-----------------------------------------------------------------------------
    
void STDMETHODCALLTYPE ID3D11DeviceContextProxy::IASetInputLayout( 
    __in_opt ID3D11InputLayout *pInputLayout)
{
    m_device->IASetInputLayout(pInputLayout);
}

//-----------------------------------------------------------------------------
    
void STDMETHODCALLTYPE ID3D11DeviceContextProxy::IASetVertexBuffers( 
    __in_range( 0, D3D11_IA_VERTEX_INPUT_RESOURCE_SLOT_COUNT - 1 ) UINT StartSlot,
    __in_range( 0, D3D11_IA_VERTEX_INPUT_RESOURCE_SLOT_COUNT - StartSlot ) UINT NumBuffers,
    __in_ecount(NumBuffers) ID3D11Buffer *const *ppVertexBuffers,
    __in_ecount(NumBuffers) const UINT *pStrides,
    __in_ecount(NumBuffers) const UINT *pOffsets)
{
    m_device->IASetVertexBuffers(StartSlot, NumBuffers, ppVertexBuffers, pStrides, pOffsets);
}

//-----------------------------------------------------------------------------
    
void STDMETHODCALLTYPE ID3D11DeviceContextProxy::IASetIndexBuffer( 
    __in_opt ID3D11Buffer *pIndexBuffer,
    __in DXGI_FORMAT Format,
    __in UINT Offset)
{
    m_device->IASetIndexBuffer(pIndexBuffer, Format, Offset);
}

//-----------------------------------------------------------------------------
    
void STDMETHODCALLTYPE ID3D11DeviceContextProxy::DrawIndexedInstanced( 
    __in UINT IndexCountPerInstance,
    __in UINT InstanceCount,
    __in UINT StartIndexLocation,
    __in INT BaseVertexLocation,
    __in UINT StartInstanceLocation)
{
    m_device->DrawIndexedInstanced(IndexCountPerInstance, InstanceCount, StartIndexLocation, BaseVertexLocation, StartInstanceLocation);
}

//-----------------------------------------------------------------------------
    
void STDMETHODCALLTYPE ID3D11DeviceContextProxy::DrawInstanced( 
    __in UINT VertexCountPerInstance,
    __in UINT InstanceCount,
    __in UINT StartVertexLocation,
    __in UINT StartInstanceLocation)
{
    m_device->DrawInstanced(VertexCountPerInstance, InstanceCount, StartVertexLocation, StartInstanceLocation);
}

//-----------------------------------------------------------------------------
    
void STDMETHODCALLTYPE ID3D11DeviceContextProxy::GSSetConstantBuffers( 
    __in_range( 0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT - 1 ) UINT StartSlot,
    __in_range( 0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT - StartSlot ) UINT NumBuffers,
    __in_ecount(NumBuffers) ID3D11Buffer *const *ppConstantBuffers)
{
    m_device->GSSetConstantBuffers(StartSlot, NumBuffers, ppConstantBuffers);
}

//-----------------------------------------------------------------------------
    
void STDMETHODCALLTYPE ID3D11DeviceContextProxy::GSSetShader( 
    __in_opt ID3D11GeometryShader *pShader,
    __in_ecount_opt(NumClassInstances) ID3D11ClassInstance *const *ppClassInstances,
    UINT NumClassInstances)
{
    m_device->GSSetShader(pShader, ppClassInstances, NumClassInstances);
}

//-----------------------------------------------------------------------------
    
void STDMETHODCALLTYPE ID3D11DeviceContextProxy::IASetPrimitiveTopology( 
    __in D3D11_PRIMITIVE_TOPOLOGY Topology)
{
    m_device->IASetPrimitiveTopology(Topology);
}

//-----------------------------------------------------------------------------
    
void STDMETHODCALLTYPE ID3D11DeviceContextProxy::VSSetShaderResources( 
    __in_range( 0, D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT - 1 ) UINT StartSlot,
    __in_range( 0, D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT - StartSlot ) UINT NumViews,
    __in_ecount(NumViews) ID3D11ShaderResourceView *const *ppShaderResourceViews)
{
    m_device->VSSetShaderResources(StartSlot, NumViews, ppShaderResourceViews);
}

//-----------------------------------------------------------------------------
    
void STDMETHODCALLTYPE ID3D11DeviceContextProxy::VSSetSamplers( 
    __in_range( 0, D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT - 1 ) UINT StartSlot,
    __in_range( 0, D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT - StartSlot ) UINT NumSamplers,
    __in_ecount(NumSamplers) ID3D11SamplerState *const *ppSamplers)
{
    m_device->VSSetSamplers(StartSlot, NumSamplers, ppSamplers);
}

//-----------------------------------------------------------------------------
    
void STDMETHODCALLTYPE ID3D11DeviceContextProxy::Begin( 
    __in ID3D11Asynchronous *pAsync)
{
    m_device->Begin(pAsync);
}

//-----------------------------------------------------------------------------
    
void STDMETHODCALLTYPE ID3D11DeviceContextProxy::End( 
    __in ID3D11Asynchronous *pAsync)
{
    m_device->End(pAsync);
}

//-----------------------------------------------------------------------------
    
HRESULT STDMETHODCALLTYPE ID3D11DeviceContextProxy::GetData( 
    __in ID3D11Asynchronous *pAsync,
    __out_bcount_opt( DataSize ) void *pData,
    __in UINT DataSize,
    __in UINT GetDataFlags)
{
    Log::print() << "GetData\n";
    return m_device->GetData(pAsync, pData, DataSize, GetDataFlags);
}

//-----------------------------------------------------------------------------
    
void STDMETHODCALLTYPE ID3D11DeviceContextProxy::SetPredication( 
    __in_opt ID3D11Predicate *pPredicate,
    __in BOOL PredicateValue)
{
    m_device->SetPredication(pPredicate, PredicateValue);
}

//-----------------------------------------------------------------------------
    
void STDMETHODCALLTYPE ID3D11DeviceContextProxy::GSSetShaderResources( 
    __in_range( 0, D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT - 1 ) UINT StartSlot,
    __in_range( 0, D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT - StartSlot ) UINT NumViews,
    __in_ecount(NumViews) ID3D11ShaderResourceView *const *ppShaderResourceViews)
{
    m_device->GSSetShaderResources(StartSlot, NumViews, ppShaderResourceViews);
}

//-----------------------------------------------------------------------------
    
void STDMETHODCALLTYPE ID3D11DeviceContextProxy::GSSetSamplers( 
    __in_range( 0, D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT - 1 ) UINT StartSlot,
    __in_range( 0, D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT - StartSlot ) UINT NumSamplers,
    __in_ecount(NumSamplers) ID3D11SamplerState *const *ppSamplers)
{
    m_device->GSSetSamplers(StartSlot, NumSamplers, ppSamplers);
}

//-----------------------------------------------------------------------------
    
void STDMETHODCALLTYPE ID3D11DeviceContextProxy::OMSetRenderTargets( 
    __in_range( 0, D3D11_SIMULTANEOUS_RENDER_TARGET_COUNT ) UINT NumViews,
    __in_ecount_opt(NumViews) ID3D11RenderTargetView *const *ppRenderTargetViews,
    __in_opt ID3D11DepthStencilView *pDepthStencilView)
{
    m_device->OMSetRenderTargets(NumViews, ppRenderTargetViews, pDepthStencilView);
}

//-----------------------------------------------------------------------------
    
void STDMETHODCALLTYPE ID3D11DeviceContextProxy::OMSetRenderTargetsAndUnorderedAccessViews( 
    __in UINT NumRTVs,
    __in_ecount_opt(NumRTVs) ID3D11RenderTargetView *const *ppRenderTargetViews,
    __in_opt ID3D11DepthStencilView *pDepthStencilView,
    __in_range( 0, D3D11_PS_CS_UAV_REGISTER_COUNT - 1 ) UINT UAVStartSlot,
    __in UINT NumUAVs,
    __in_ecount_opt(NumUAVs) ID3D11UnorderedAccessView *const *ppUnorderedAccessViews,
    __in_ecount_opt(NumUAVs) const UINT *pUAVInitialCounts)
{
    m_device->OMSetRenderTargetsAndUnorderedAccessViews(NumRTVs, ppRenderTargetViews, pDepthStencilView, UAVStartSlot, NumUAVs, ppUnorderedAccessViews, pUAVInitialCounts);
}

//-----------------------------------------------------------------------------
    
void STDMETHODCALLTYPE ID3D11DeviceContextProxy::OMSetBlendState( 
    __in_opt ID3D11BlendState *pBlendState,
    __in_opt const FLOAT BlendFactor[ 4 ],
    __in UINT SampleMask)
{
    m_device->OMSetBlendState(pBlendState, BlendFactor, SampleMask);
}

//-----------------------------------------------------------------------------
    
void STDMETHODCALLTYPE ID3D11DeviceContextProxy::OMSetDepthStencilState( 
    __in_opt ID3D11DepthStencilState *pDepthStencilState,
    __in UINT StencilRef)
{
    m_device->OMSetDepthStencilState(pDepthStencilState, StencilRef);
}

//-----------------------------------------------------------------------------
    
void STDMETHODCALLTYPE ID3D11DeviceContextProxy::SOSetTargets( 
    __in_range( 0, D3D11_SO_BUFFER_SLOT_COUNT) UINT NumBuffers,
    __in_ecount_opt(NumBuffers) ID3D11Buffer *const *ppSOTargets,
    __in_ecount_opt(NumBuffers) const UINT *pOffsets)
{
    m_device->SOSetTargets(NumBuffers, ppSOTargets, pOffsets);
}

//-----------------------------------------------------------------------------
    
void STDMETHODCALLTYPE ID3D11DeviceContextProxy::DrawAuto()
{
    m_device->DrawAuto();
}

//-----------------------------------------------------------------------------
    
void STDMETHODCALLTYPE ID3D11DeviceContextProxy::DrawIndexedInstancedIndirect( 
    __in ID3D11Buffer *pBufferForArgs,
    __in UINT AlignedByteOffsetForArgs)
{
    m_device->DrawIndexedInstancedIndirect(pBufferForArgs, AlignedByteOffsetForArgs);
}

//-----------------------------------------------------------------------------
    
void STDMETHODCALLTYPE ID3D11DeviceContextProxy::DrawInstancedIndirect( 
    __in ID3D11Buffer *pBufferForArgs,
    __in UINT AlignedByteOffsetForArgs)
{
    m_device->DrawInstancedIndirect(pBufferForArgs, AlignedByteOffsetForArgs);
}

//-----------------------------------------------------------------------------
    
void STDMETHODCALLTYPE ID3D11DeviceContextProxy::Dispatch( 
    __in UINT ThreadGroupCountX,
    __in UINT ThreadGroupCountY,
    __in UINT ThreadGroupCountZ)
{
    m_device->Dispatch(ThreadGroupCountX, ThreadGroupCountY, ThreadGroupCountZ);
}

//-----------------------------------------------------------------------------
    
void STDMETHODCALLTYPE ID3D11DeviceContextProxy::DispatchIndirect( 
    __in ID3D11Buffer *pBufferForArgs,
    __in UINT AlignedByteOffsetForArgs)
{
    m_device->DispatchIndirect(pBufferForArgs, AlignedByteOffsetForArgs);
}

//-----------------------------------------------------------------------------
    
void STDMETHODCALLTYPE ID3D11DeviceContextProxy::RSSetState( 
    __in_opt ID3D11RasterizerState *pRasterizerState)
{
    m_device->RSSetState(pRasterizerState);
}

//-----------------------------------------------------------------------------
    
void STDMETHODCALLTYPE ID3D11DeviceContextProxy::RSSetViewports( 
    __in_range(0, D3D11_VIEWPORT_AND_SCISSORRECT_OBJECT_COUNT_PER_PIPELINE) UINT NumViewports,
    __in_ecount_opt(NumViewports) const D3D11_VIEWPORT *pViewports)
{
    m_device->RSSetViewports(NumViewports, pViewports);
}

//-----------------------------------------------------------------------------
    
void STDMETHODCALLTYPE ID3D11DeviceContextProxy::RSSetScissorRects( 
    __in_range(0, D3D11_VIEWPORT_AND_SCISSORRECT_OBJECT_COUNT_PER_PIPELINE) UINT NumRects,
    __in_ecount_opt(NumRects) const D3D11_RECT *pRects)
{
    m_device->RSSetScissorRects(NumRects, pRects);
}

//-----------------------------------------------------------------------------
    
void STDMETHODCALLTYPE ID3D11DeviceContextProxy::CopySubresourceRegion( 
    __in ID3D11Resource *pDstResource,
    __in UINT DstSubresource,
    __in UINT DstX,
    __in UINT DstY,
    __in UINT DstZ,
    __in ID3D11Resource *pSrcResource,
    __in UINT SrcSubresource,
    __in_opt const D3D11_BOX *pSrcBox)
{
    m_device->CopySubresourceRegion(pDstResource, DstSubresource, DstX, DstY, DstZ, pSrcResource, SrcSubresource, pSrcBox);
}

//-----------------------------------------------------------------------------
    
void STDMETHODCALLTYPE ID3D11DeviceContextProxy::CopyResource( 
    __in ID3D11Resource *pDstResource,
    __in ID3D11Resource *pSrcResource)
{
    m_device->CopyResource(pDstResource, pSrcResource);
}

//-----------------------------------------------------------------------------
    
void STDMETHODCALLTYPE ID3D11DeviceContextProxy::UpdateSubresource( 
    __in ID3D11Resource *pDstResource,
    __in UINT DstSubresource,
    __in_opt const D3D11_BOX *pDstBox,
    __in const void *pSrcData,
    __in UINT SrcRowPitch,
    __in UINT SrcDepthPitch)
{
    m_device->UpdateSubresource(pDstResource, DstSubresource, pDstBox, pSrcData, SrcRowPitch, SrcDepthPitch);
}

//-----------------------------------------------------------------------------
    
void STDMETHODCALLTYPE ID3D11DeviceContextProxy::CopyStructureCount( 
    __in ID3D11Buffer *pDstBuffer,
    __in UINT DstAlignedByteOffset,
    __in ID3D11UnorderedAccessView *pSrcView)
{
    m_device->CopyStructureCount(pDstBuffer, DstAlignedByteOffset, pSrcView);
}

//-----------------------------------------------------------------------------
    
void STDMETHODCALLTYPE ID3D11DeviceContextProxy::ClearRenderTargetView( 
    __in ID3D11RenderTargetView *pRenderTargetView,
    __in const FLOAT ColorRGBA[ 4 ])
{
    Log::print() << "ClearRenderTargetView\n";
    m_device->ClearRenderTargetView(pRenderTargetView, ColorRGBA);
}

//-----------------------------------------------------------------------------
    
void STDMETHODCALLTYPE ID3D11DeviceContextProxy::ClearUnorderedAccessViewUint( 
    __in ID3D11UnorderedAccessView *pUnorderedAccessView,
    __in const UINT Values[ 4 ])
{
    m_device->ClearUnorderedAccessViewUint(pUnorderedAccessView, Values);
}

//-----------------------------------------------------------------------------
    
void STDMETHODCALLTYPE ID3D11DeviceContextProxy::ClearUnorderedAccessViewFloat( 
    __in ID3D11UnorderedAccessView *pUnorderedAccessView,
    __in const FLOAT Values[ 4 ])
{
    m_device->ClearUnorderedAccessViewFloat(pUnorderedAccessView, Values);
}

//-----------------------------------------------------------------------------
    
void STDMETHODCALLTYPE ID3D11DeviceContextProxy::ClearDepthStencilView( 
    __in ID3D11DepthStencilView *pDepthStencilView,
    __in UINT ClearFlags,
    __in FLOAT Depth,
    __in UINT8 Stencil)
{
    m_device->ClearDepthStencilView(pDepthStencilView, ClearFlags, Depth, Stencil);
}

//-----------------------------------------------------------------------------
    
void STDMETHODCALLTYPE ID3D11DeviceContextProxy::GenerateMips( 
    __in ID3D11ShaderResourceView *pShaderResourceView)
{
    m_device->GenerateMips(pShaderResourceView);
}

//-----------------------------------------------------------------------------
    
void STDMETHODCALLTYPE ID3D11DeviceContextProxy::SetResourceMinLOD( 
    __in ID3D11Resource *pResource,
    FLOAT MinLOD)
{
    m_device->SetResourceMinLOD(pResource, MinLOD);
}

//-----------------------------------------------------------------------------
    
FLOAT STDMETHODCALLTYPE ID3D11DeviceContextProxy::GetResourceMinLOD( 
    __in ID3D11Resource *pResource)
{
    return m_device->GetResourceMinLOD(pResource);
}

//-----------------------------------------------------------------------------
    
void STDMETHODCALLTYPE ID3D11DeviceContextProxy::ResolveSubresource( 
    __in ID3D11Resource *pDstResource,
    __in UINT DstSubresource,
    __in ID3D11Resource *pSrcResource,
    __in UINT SrcSubresource,
    __in DXGI_FORMAT Format)
{
    m_device->ResolveSubresource(pDstResource, DstSubresource, pSrcResource, SrcSubresource, Format);
}

//-----------------------------------------------------------------------------
    
void STDMETHODCALLTYPE ID3D11DeviceContextProxy::ExecuteCommandList( 
    __in ID3D11CommandList *pCommandList,
    BOOL RestoreContextState)
{
    m_device->ExecuteCommandList(pCommandList, RestoreContextState);
}

//-----------------------------------------------------------------------------

void STDMETHODCALLTYPE ID3D11DeviceContextProxy::HSSetShaderResources( 
    __in_range( 0, D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT - 1 ) UINT StartSlot,
    __in_range( 0, D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT - StartSlot ) UINT NumViews,
    __in_ecount(NumViews) ID3D11ShaderResourceView *const *ppShaderResourceViews)
{
    m_device->HSSetShaderResources(StartSlot, NumViews, ppShaderResourceViews);
}

//-----------------------------------------------------------------------------
    
void STDMETHODCALLTYPE ID3D11DeviceContextProxy::HSSetShader( 
    __in_opt ID3D11HullShader *pHullShader,
    __in_ecount_opt(NumClassInstances) ID3D11ClassInstance *const *ppClassInstances,
    UINT NumClassInstances)
{
    m_device->HSSetShader(pHullShader, ppClassInstances, NumClassInstances);
}

//-----------------------------------------------------------------------------
    
void STDMETHODCALLTYPE ID3D11DeviceContextProxy::HSSetSamplers( 
    __in_range( 0, D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT - 1 ) UINT StartSlot,
    __in_range( 0, D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT - StartSlot ) UINT NumSamplers,
    __in_ecount(NumSamplers) ID3D11SamplerState *const *ppSamplers)
{
    m_device->HSSetSamplers(StartSlot, NumSamplers, ppSamplers);
}

//-----------------------------------------------------------------------------
    
void STDMETHODCALLTYPE ID3D11DeviceContextProxy::HSSetConstantBuffers( 
    __in_range( 0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT - 1 ) UINT StartSlot,
    __in_range( 0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT - StartSlot ) UINT NumBuffers,
    __in_ecount(NumBuffers) ID3D11Buffer *const *ppConstantBuffers)
{
    m_device->HSSetConstantBuffers(StartSlot, NumBuffers, ppConstantBuffers);
}

//-----------------------------------------------------------------------------
    
void STDMETHODCALLTYPE ID3D11DeviceContextProxy::DSSetShaderResources( 
    __in_range( 0, D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT - 1 ) UINT StartSlot,
    __in_range( 0, D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT - StartSlot ) UINT NumViews,
    __in_ecount(NumViews) ID3D11ShaderResourceView *const *ppShaderResourceViews)
{
    m_device->DSSetShaderResources(StartSlot, NumViews, ppShaderResourceViews);
}

//-----------------------------------------------------------------------------

void STDMETHODCALLTYPE ID3D11DeviceContextProxy::DSSetShader( 
    __in_opt ID3D11DomainShader *pDomainShader,
    __in_ecount_opt(NumClassInstances) ID3D11ClassInstance *const *ppClassInstances,
    UINT NumClassInstances)
{
    m_device->DSSetShader(pDomainShader, ppClassInstances, NumClassInstances);
}

//-----------------------------------------------------------------------------

void STDMETHODCALLTYPE ID3D11DeviceContextProxy::DSSetSamplers( 
    __in_range( 0, D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT - 1 ) UINT StartSlot,
    __in_range( 0, D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT - StartSlot ) UINT NumSamplers,
    __in_ecount(NumSamplers) ID3D11SamplerState *const *ppSamplers)
{
    m_device->DSSetSamplers(StartSlot, NumSamplers, ppSamplers);
}

//-----------------------------------------------------------------------------

void STDMETHODCALLTYPE ID3D11DeviceContextProxy::DSSetConstantBuffers( 
    __in_range( 0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT - 1 ) UINT StartSlot,
    __in_range( 0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT - StartSlot ) UINT NumBuffers,
    __in_ecount(NumBuffers) ID3D11Buffer *const *ppConstantBuffers)
{
    m_device->DSSetConstantBuffers(StartSlot, NumBuffers, ppConstantBuffers);
}

//-----------------------------------------------------------------------------

void STDMETHODCALLTYPE ID3D11DeviceContextProxy::CSSetShaderResources( 
    __in_range( 0, D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT - 1 ) UINT StartSlot,
    __in_range( 0, D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT - StartSlot ) UINT NumViews,
    __in_ecount(NumViews) ID3D11ShaderResourceView *const *ppShaderResourceViews)
{
    m_device->CSSetShaderResources(StartSlot, NumViews, ppShaderResourceViews);
}

//-----------------------------------------------------------------------------

void STDMETHODCALLTYPE ID3D11DeviceContextProxy::CSSetUnorderedAccessViews( 
    __in_range( 0, D3D11_PS_CS_UAV_REGISTER_COUNT - 1 ) UINT StartSlot,
    __in_range( 0, D3D11_PS_CS_UAV_REGISTER_COUNT - StartSlot ) UINT NumUAVs,
    __in_ecount(NumUAVs) ID3D11UnorderedAccessView *const *ppUnorderedAccessViews,
    __in_ecount(NumUAVs) const UINT *pUAVInitialCounts)
{
    m_device->CSSetUnorderedAccessViews(StartSlot, NumUAVs, ppUnorderedAccessViews, pUAVInitialCounts);
}

//-----------------------------------------------------------------------------

void STDMETHODCALLTYPE ID3D11DeviceContextProxy::CSSetShader( 
    __in_opt ID3D11ComputeShader *pComputeShader,
    __in_ecount_opt(NumClassInstances) ID3D11ClassInstance *const *ppClassInstances,
    UINT NumClassInstances)
{
    m_device->CSSetShader(pComputeShader, ppClassInstances, NumClassInstances);
}

//-----------------------------------------------------------------------------

void STDMETHODCALLTYPE ID3D11DeviceContextProxy::CSSetSamplers( 
    __in_range( 0, D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT - 1 ) UINT StartSlot,
    __in_range( 0, D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT - StartSlot ) UINT NumSamplers,
    __in_ecount(NumSamplers) ID3D11SamplerState *const *ppSamplers)
{
    m_device->CSSetSamplers(StartSlot, NumSamplers, ppSamplers);
}

//-----------------------------------------------------------------------------

void STDMETHODCALLTYPE ID3D11DeviceContextProxy::CSSetConstantBuffers( 
    __in_range( 0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT - 1 ) UINT StartSlot,
    __in_range( 0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT - StartSlot ) UINT NumBuffers,
    __in_ecount(NumBuffers) ID3D11Buffer *const *ppConstantBuffers)
{
    m_device->CSSetConstantBuffers(StartSlot, NumBuffers, ppConstantBuffers);
}

//-----------------------------------------------------------------------------

void STDMETHODCALLTYPE ID3D11DeviceContextProxy::VSGetConstantBuffers( 
    __in_range( 0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT - 1 ) UINT StartSlot,
    __in_range( 0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT - StartSlot ) UINT NumBuffers,
    __out_ecount(NumBuffers) ID3D11Buffer **ppConstantBuffers)
{
    m_device->VSGetConstantBuffers(StartSlot, NumBuffers, ppConstantBuffers);
}

//-----------------------------------------------------------------------------

void STDMETHODCALLTYPE ID3D11DeviceContextProxy::PSGetShaderResources( 
    __in_range( 0, D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT - 1 ) UINT StartSlot,
    __in_range( 0, D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT - StartSlot ) UINT NumViews,
    __out_ecount(NumViews) ID3D11ShaderResourceView **ppShaderResourceViews)
{
    m_device->PSGetShaderResources(StartSlot, NumViews, ppShaderResourceViews);
}

//-----------------------------------------------------------------------------

void STDMETHODCALLTYPE ID3D11DeviceContextProxy::PSGetShader( 
    __out_opt ID3D11PixelShader **ppPixelShader,
    __out_ecount_opt(*pNumClassInstances) ID3D11ClassInstance **ppClassInstances,
    __inout_opt UINT *pNumClassInstances)
{
    m_device->PSGetShader(ppPixelShader, ppClassInstances, pNumClassInstances);
}

//-----------------------------------------------------------------------------

void STDMETHODCALLTYPE ID3D11DeviceContextProxy::PSGetSamplers( 
    __in_range( 0, D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT - 1 ) UINT StartSlot,
    __in_range( 0, D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT - StartSlot ) UINT NumSamplers,
    __out_ecount(NumSamplers) ID3D11SamplerState **ppSamplers)
{
    m_device->PSGetSamplers(StartSlot, NumSamplers, ppSamplers);
}

//-----------------------------------------------------------------------------

void STDMETHODCALLTYPE ID3D11DeviceContextProxy::VSGetShader( 
    __out_opt ID3D11VertexShader **ppVertexShader,
    __out_ecount_opt(*pNumClassInstances) ID3D11ClassInstance **ppClassInstances,
    __inout_opt UINT *pNumClassInstances)
{
    m_device->VSGetShader(ppVertexShader, ppClassInstances, pNumClassInstances);
}

//-----------------------------------------------------------------------------

void STDMETHODCALLTYPE ID3D11DeviceContextProxy::PSGetConstantBuffers( 
    __in_range( 0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT - 1 ) UINT StartSlot,
    __in_range( 0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT - StartSlot ) UINT NumBuffers,
    __out_ecount(NumBuffers) ID3D11Buffer **ppConstantBuffers)
{
    m_device->PSGetConstantBuffers(StartSlot, NumBuffers, ppConstantBuffers);
}

//-----------------------------------------------------------------------------

void STDMETHODCALLTYPE ID3D11DeviceContextProxy::IAGetInputLayout( 
    __out ID3D11InputLayout **ppInputLayout)
{
    m_device->IAGetInputLayout(ppInputLayout);
}

//-----------------------------------------------------------------------------

void STDMETHODCALLTYPE ID3D11DeviceContextProxy::IAGetVertexBuffers( 
    __in_range( 0, D3D11_IA_VERTEX_INPUT_RESOURCE_SLOT_COUNT - 1 ) UINT StartSlot,
    __in_range( 0, D3D11_IA_VERTEX_INPUT_RESOURCE_SLOT_COUNT - StartSlot ) UINT NumBuffers,
    __out_ecount_opt(NumBuffers) ID3D11Buffer **ppVertexBuffers,
    __out_ecount_opt(NumBuffers) UINT *pStrides,
    __out_ecount_opt(NumBuffers) UINT *pOffsets)
{
    m_device->IAGetVertexBuffers(StartSlot, NumBuffers, ppVertexBuffers, pStrides, pOffsets);
}

//-----------------------------------------------------------------------------

void STDMETHODCALLTYPE ID3D11DeviceContextProxy::IAGetIndexBuffer( 
    __out_opt ID3D11Buffer **pIndexBuffer,
    __out_opt DXGI_FORMAT *Format,
    __out_opt UINT *Offset)
{
    m_device->IAGetIndexBuffer(pIndexBuffer, Format, Offset);
}

//-----------------------------------------------------------------------------

void STDMETHODCALLTYPE ID3D11DeviceContextProxy::GSGetConstantBuffers( 
    __in_range( 0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT - 1 ) UINT StartSlot,
    __in_range( 0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT - StartSlot ) UINT NumBuffers,
    __out_ecount(NumBuffers) ID3D11Buffer **ppConstantBuffers)
{
    m_device->GSGetConstantBuffers(StartSlot, NumBuffers, ppConstantBuffers);
}

//-----------------------------------------------------------------------------

void STDMETHODCALLTYPE ID3D11DeviceContextProxy::GSGetShader( 
    __out_opt ID3D11GeometryShader **ppGeometryShader,
    __out_ecount_opt(*pNumClassInstances) ID3D11ClassInstance **ppClassInstances,
    __inout_opt UINT *pNumClassInstances)
{
    m_device->GSGetShader(ppGeometryShader, ppClassInstances, pNumClassInstances);
}

//-----------------------------------------------------------------------------

void STDMETHODCALLTYPE ID3D11DeviceContextProxy::IAGetPrimitiveTopology( 
    __out D3D11_PRIMITIVE_TOPOLOGY *pTopology)
{
    m_device->IAGetPrimitiveTopology(pTopology);
}

//-----------------------------------------------------------------------------

void STDMETHODCALLTYPE ID3D11DeviceContextProxy::VSGetShaderResources( 
    __in_range( 0, D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT - 1 ) UINT StartSlot,
    __in_range( 0, D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT - StartSlot ) UINT NumViews,
    __out_ecount(NumViews) ID3D11ShaderResourceView **ppShaderResourceViews)
{
    m_device->VSGetShaderResources(StartSlot, NumViews, ppShaderResourceViews);
}

//-----------------------------------------------------------------------------

void STDMETHODCALLTYPE ID3D11DeviceContextProxy::VSGetSamplers( 
    __in_range( 0, D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT - 1 ) UINT StartSlot,
    __in_range( 0, D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT - StartSlot ) UINT NumSamplers,
    __out_ecount(NumSamplers) ID3D11SamplerState **ppSamplers)
{
    m_device->VSGetSamplers(StartSlot, NumSamplers, ppSamplers);
}

//-----------------------------------------------------------------------------

void STDMETHODCALLTYPE ID3D11DeviceContextProxy::GetPredication( 
    __out_opt ID3D11Predicate **ppPredicate,
    __out_opt BOOL *pPredicateValue)
{
    m_device->GetPredication(ppPredicate, pPredicateValue);
}

//-----------------------------------------------------------------------------

void STDMETHODCALLTYPE ID3D11DeviceContextProxy::GSGetShaderResources( 
    __in_range( 0, D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT - 1 ) UINT StartSlot,
    __in_range( 0, D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT - StartSlot ) UINT NumViews,
    __out_ecount(NumViews) ID3D11ShaderResourceView **ppShaderResourceViews)
{
    m_device->GSGetShaderResources(StartSlot, NumViews, ppShaderResourceViews);
}

//-----------------------------------------------------------------------------

void STDMETHODCALLTYPE ID3D11DeviceContextProxy::GSGetSamplers( 
    __in_range( 0, D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT - 1 ) UINT StartSlot,
    __in_range( 0, D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT - StartSlot ) UINT NumSamplers,
    __out_ecount(NumSamplers) ID3D11SamplerState **ppSamplers)
{
    m_device->GSGetSamplers(StartSlot, NumSamplers, ppSamplers);
}

//-----------------------------------------------------------------------------

void STDMETHODCALLTYPE ID3D11DeviceContextProxy::OMGetRenderTargets( 
    __in_range( 0, D3D11_SIMULTANEOUS_RENDER_TARGET_COUNT ) UINT NumViews,
    __out_ecount_opt(NumViews) ID3D11RenderTargetView **ppRenderTargetViews,
    __out_opt ID3D11DepthStencilView **ppDepthStencilView)
{
    m_device->OMGetRenderTargets(NumViews, ppRenderTargetViews, ppDepthStencilView);
}

//-----------------------------------------------------------------------------

void STDMETHODCALLTYPE ID3D11DeviceContextProxy::OMGetRenderTargetsAndUnorderedAccessViews( 
    __in_range( 0, D3D11_SIMULTANEOUS_RENDER_TARGET_COUNT ) UINT NumRTVs,
    __out_ecount_opt(NumRTVs) ID3D11RenderTargetView **ppRenderTargetViews,
    __out_opt ID3D11DepthStencilView **ppDepthStencilView,
    __in_range( 0, D3D11_PS_CS_UAV_REGISTER_COUNT - 1 ) UINT UAVStartSlot,
    __in_range( 0, D3D11_PS_CS_UAV_REGISTER_COUNT - UAVStartSlot ) UINT NumUAVs,
    __out_ecount_opt(NumUAVs) ID3D11UnorderedAccessView **ppUnorderedAccessViews)
{
    m_device->OMGetRenderTargetsAndUnorderedAccessViews(NumRTVs, ppRenderTargetViews, ppDepthStencilView, UAVStartSlot, NumUAVs, ppUnorderedAccessViews);
}

//-----------------------------------------------------------------------------

void STDMETHODCALLTYPE ID3D11DeviceContextProxy::OMGetBlendState( 
    __out_opt ID3D11BlendState **ppBlendState,
    __out_opt FLOAT BlendFactor[ 4 ],
    __out_opt UINT *pSampleMask)
{
    m_device->OMGetBlendState(ppBlendState, BlendFactor, pSampleMask);
}

//-----------------------------------------------------------------------------

void STDMETHODCALLTYPE ID3D11DeviceContextProxy::OMGetDepthStencilState( 
    __out_opt ID3D11DepthStencilState **ppDepthStencilState,
    __out_opt UINT *pStencilRef)
{
    m_device->OMGetDepthStencilState(ppDepthStencilState, pStencilRef);
}

//-----------------------------------------------------------------------------

void STDMETHODCALLTYPE ID3D11DeviceContextProxy::SOGetTargets( 
    __in_range( 0, D3D11_SO_BUFFER_SLOT_COUNT ) UINT NumBuffers,
    __out_ecount(NumBuffers) ID3D11Buffer **ppSOTargets)
{
    m_device->SOGetTargets(NumBuffers, ppSOTargets);
}

//-----------------------------------------------------------------------------

void STDMETHODCALLTYPE ID3D11DeviceContextProxy::RSGetState( 
    __out ID3D11RasterizerState **ppRasterizerState)
{
    m_device->RSGetState(ppRasterizerState);
}

//-----------------------------------------------------------------------------

void STDMETHODCALLTYPE ID3D11DeviceContextProxy::RSGetViewports( 
    __inout /*_range(0, D3D11_VIEWPORT_AND_SCISSORRECT_OBJECT_COUNT_PER_PIPELINE )*/  UINT *pNumViewports,
    __out_ecount_opt(*pNumViewports) D3D11_VIEWPORT *pViewports)
{
    m_device->RSGetViewports(pNumViewports, pViewports);
}

//-----------------------------------------------------------------------------

void STDMETHODCALLTYPE ID3D11DeviceContextProxy::RSGetScissorRects( 
    __inout /*_range(0, D3D11_VIEWPORT_AND_SCISSORRECT_OBJECT_COUNT_PER_PIPELINE )*/  UINT *pNumRects,
    __out_ecount_opt(*pNumRects) D3D11_RECT *pRects)
{
    m_device->RSGetScissorRects(pNumRects, pRects);
}

//-----------------------------------------------------------------------------

void STDMETHODCALLTYPE ID3D11DeviceContextProxy::HSGetShaderResources( 
    __in_range( 0, D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT - 1 ) UINT StartSlot,
    __in_range( 0, D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT - StartSlot ) UINT NumViews,
    __out_ecount(NumViews) ID3D11ShaderResourceView **ppShaderResourceViews)
{
    m_device->HSGetShaderResources(StartSlot, NumViews, ppShaderResourceViews);
}

//-----------------------------------------------------------------------------

void STDMETHODCALLTYPE ID3D11DeviceContextProxy::HSGetShader( 
    __out_opt ID3D11HullShader **ppHullShader,
    __out_ecount_opt(*pNumClassInstances) ID3D11ClassInstance **ppClassInstances,
    __inout_opt UINT *pNumClassInstances)
{
    m_device->HSGetShader(ppHullShader, ppClassInstances, pNumClassInstances);
}

//-----------------------------------------------------------------------------

void STDMETHODCALLTYPE ID3D11DeviceContextProxy::HSGetSamplers( 
    __in_range( 0, D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT - 1 ) UINT StartSlot,
    __in_range( 0, D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT - StartSlot ) UINT NumSamplers,
    __out_ecount(NumSamplers) ID3D11SamplerState **ppSamplers)
{
    m_device->HSGetSamplers(StartSlot, NumSamplers, ppSamplers);
}

//-----------------------------------------------------------------------------

void STDMETHODCALLTYPE ID3D11DeviceContextProxy::HSGetConstantBuffers( 
    __in_range( 0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT - 1 ) UINT StartSlot,
    __in_range( 0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT - StartSlot ) UINT NumBuffers,
    __out_ecount(NumBuffers) ID3D11Buffer **ppConstantBuffers)
{
    m_device->HSGetConstantBuffers(StartSlot, NumBuffers, ppConstantBuffers);
}

//-----------------------------------------------------------------------------

void STDMETHODCALLTYPE ID3D11DeviceContextProxy::DSGetShaderResources( 
    __in_range( 0, D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT - 1 ) UINT StartSlot,
    __in_range( 0, D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT - StartSlot ) UINT NumViews,
    __out_ecount(NumViews) ID3D11ShaderResourceView **ppShaderResourceViews)
{
    m_device->DSGetShaderResources(StartSlot, NumViews, ppShaderResourceViews);
}

//-----------------------------------------------------------------------------

void STDMETHODCALLTYPE ID3D11DeviceContextProxy::DSGetShader( 
    __out_opt ID3D11DomainShader **ppDomainShader,
    __out_ecount_opt(*pNumClassInstances) ID3D11ClassInstance **ppClassInstances,
    __inout_opt UINT *pNumClassInstances)
{
    m_device->DSGetShader(ppDomainShader, ppClassInstances, pNumClassInstances);
}

//-----------------------------------------------------------------------------

void STDMETHODCALLTYPE ID3D11DeviceContextProxy::DSGetSamplers( 
    __in_range( 0, D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT - 1 ) UINT StartSlot,
    __in_range( 0, D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT - StartSlot ) UINT NumSamplers,
    __out_ecount(NumSamplers) ID3D11SamplerState **ppSamplers)
{
    m_device->DSGetSamplers(StartSlot, NumSamplers, ppSamplers);
}

//-----------------------------------------------------------------------------

void STDMETHODCALLTYPE ID3D11DeviceContextProxy::DSGetConstantBuffers( 
    __in_range( 0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT - 1 ) UINT StartSlot,
    __in_range( 0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT - StartSlot ) UINT NumBuffers,
    __out_ecount(NumBuffers) ID3D11Buffer **ppConstantBuffers)
{
    m_device->DSGetConstantBuffers(StartSlot, NumBuffers, ppConstantBuffers);
}

//-----------------------------------------------------------------------------

void STDMETHODCALLTYPE ID3D11DeviceContextProxy::CSGetShaderResources( 
    __in_range( 0, D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT - 1 ) UINT StartSlot,
    __in_range( 0, D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT - StartSlot ) UINT NumViews,
    __out_ecount(NumViews) ID3D11ShaderResourceView **ppShaderResourceViews)
{
    m_device->CSGetShaderResources(StartSlot, NumViews, ppShaderResourceViews);
}

//-----------------------------------------------------------------------------

void STDMETHODCALLTYPE ID3D11DeviceContextProxy::CSGetUnorderedAccessViews( 
    __in_range( 0, D3D11_PS_CS_UAV_REGISTER_COUNT - 1 ) UINT StartSlot,
    __in_range( 0, D3D11_PS_CS_UAV_REGISTER_COUNT - StartSlot ) UINT NumUAVs,
    __out_ecount(NumUAVs) ID3D11UnorderedAccessView **ppUnorderedAccessViews)
{
    m_device->CSGetUnorderedAccessViews(StartSlot, NumUAVs, ppUnorderedAccessViews);
}

//-----------------------------------------------------------------------------

void STDMETHODCALLTYPE ID3D11DeviceContextProxy::CSGetShader( 
    __out_opt ID3D11ComputeShader **ppComputeShader,
    __out_ecount_opt(*pNumClassInstances) ID3D11ClassInstance **ppClassInstances,
    __inout_opt UINT *pNumClassInstances)
{
    m_device->CSGetShader(ppComputeShader, ppClassInstances, pNumClassInstances);
}

//-----------------------------------------------------------------------------

void STDMETHODCALLTYPE ID3D11DeviceContextProxy::CSGetSamplers( 
    __in_range( 0, D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT - 1 ) UINT StartSlot,
    __in_range( 0, D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT - StartSlot ) UINT NumSamplers,
    __out_ecount(NumSamplers) ID3D11SamplerState **ppSamplers)
{
    m_device->CSGetSamplers(StartSlot, NumSamplers, ppSamplers);
}

//-----------------------------------------------------------------------------

void STDMETHODCALLTYPE ID3D11DeviceContextProxy::CSGetConstantBuffers( 
    __in_range( 0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT - 1 ) UINT StartSlot,
    __in_range( 0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT - StartSlot ) UINT NumBuffers,
    __out_ecount(NumBuffers) ID3D11Buffer **ppConstantBuffers)
{
    m_device->CSGetConstantBuffers(StartSlot, NumBuffers, ppConstantBuffers);
}

//-----------------------------------------------------------------------------

void STDMETHODCALLTYPE ID3D11DeviceContextProxy::ClearState()
{
    m_device->ClearState();
}

//-----------------------------------------------------------------------------

void STDMETHODCALLTYPE ID3D11DeviceContextProxy::Flush()
{
    m_device->Flush();
}

//-----------------------------------------------------------------------------

D3D11_DEVICE_CONTEXT_TYPE STDMETHODCALLTYPE ID3D11DeviceContextProxy::GetType()
{
    return m_device->GetType();
}

//-----------------------------------------------------------------------------

UINT STDMETHODCALLTYPE ID3D11DeviceContextProxy::GetContextFlags()
{
    return m_device->GetContextFlags();
}

//-----------------------------------------------------------------------------

HRESULT STDMETHODCALLTYPE ID3D11DeviceContextProxy::FinishCommandList(
    BOOL RestoreDeferredContextState,
    __out_opt ID3D11CommandList **ppCommandList)
{
    return m_device->FinishCommandList(RestoreDeferredContextState, ppCommandList);
}

//-----------------------------------------------------------------------------

void STDMETHODCALLTYPE ID3D11DeviceContextProxy::GetDevice(
    __out ID3D11Device **ppDevice
) {
    m_device->GetDevice(ppDevice);
}

//-----------------------------------------------------------------------------

HRESULT STDMETHODCALLTYPE ID3D11DeviceContextProxy::GetPrivateData(
    __in REFGUID guid,
    __inout UINT *pDataSize,
    __out_bcount_opt( *pDataSize ) void *pData
) {
    return m_device->GetPrivateData(guid, pDataSize, pData);
}

//-----------------------------------------------------------------------------

HRESULT STDMETHODCALLTYPE ID3D11DeviceContextProxy::SetPrivateData(
    __in REFGUID guid,
    __in UINT DataSize,
    __in_bcount_opt( DataSize ) const void *pData
) {
    return m_device->SetPrivateData(guid, DataSize, pData);
}

//-----------------------------------------------------------------------------

HRESULT STDMETHODCALLTYPE ID3D11DeviceContextProxy::SetPrivateDataInterface(
    __in REFGUID guid,
    __in_opt const IUnknown *pData
) {
    return m_device->SetPrivateDataInterface(guid, pData);
}

//-----------------------------------------------------------------------------

HRESULT STDMETHODCALLTYPE ID3D11DeviceContextProxy::QueryInterface(
    REFIID riid,
    __RPC__deref_out void __RPC_FAR *__RPC_FAR *ppvObject
) {
    Log::print() << "ID3D11DeviceContextProxy::QueryInterface("
                 << GUIDtoObjectName(riid) << ")\n";
    
    return m_device->QueryInterface(riid, ppvObject);
}

//-----------------------------------------------------------------------------

ULONG STDMETHODCALLTYPE ID3D11DeviceContextProxy::AddRef()
{
    return m_device->AddRef();
}

//-----------------------------------------------------------------------------

ULONG STDMETHODCALLTYPE ID3D11DeviceContextProxy::Release()
{
    return m_device->Release();
}

//-----------------------------------------------------------------------------

#endif//SUPPORT_D3D11