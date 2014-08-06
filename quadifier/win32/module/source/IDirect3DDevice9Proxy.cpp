#include "IDirect3DDevice9Proxy.h"
#include "Log.h"
#include "DebugUtil.h"
#include "Settings.h"
using namespace hive;
using namespace std;

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

IDirect3DDevice9Proxy::IDirect3DDevice9Proxy(
    IDirect3DDevice9 *device,
    IDirect3D9 *direct3D
) :
    m_quad( device, direct3D ),
    m_device( device ),
    m_direct3D( direct3D )
{
    Log::print() << "IDirect3DDevice9Proxy("
        << device << ',' << direct3D << ")\n";

    // when the D3D device is created, call onCreateDX
    if ( !Settings::get().passThrough ) m_quad.onCreateDX();
}

//-----------------------------------------------------------------------------

IDirect3DDevice9Proxy::~IDirect3DDevice9Proxy() {
}

//-----------------------------------------------------------------------------

HRESULT IDirect3DDevice9Proxy::QueryInterface( REFIID riid, void **ppvObj)
{
    Log::print() << "IDirect3DDevice9Proxy::QueryInterface("
                 << GUIDtoObjectName(riid) << ")\n";

    HRESULT result = m_device->QueryInterface( riid, ppvObj ); 

    if ( result == S_OK ) {
        if ( riid == __uuidof(IDirect3DDevice9) ) {
            // return our proxy class (this) instead of the real one
            *ppvObj = this;
        }
    } else
        *ppvObj = 0; 

    return result; 
}

//-----------------------------------------------------------------------------

ULONG IDirect3DDevice9Proxy::AddRef()
{
    return m_device->AddRef();
}

//-----------------------------------------------------------------------------

ULONG IDirect3DDevice9Proxy::Release()
{
    return m_device->Release();
}

//-----------------------------------------------------------------------------

HRESULT IDirect3DDevice9Proxy::TestCooperativeLevel()
{
    return m_device->TestCooperativeLevel();
}

//-----------------------------------------------------------------------------

UINT IDirect3DDevice9Proxy::GetAvailableTextureMem()
{
    return m_device->GetAvailableTextureMem();
}

//-----------------------------------------------------------------------------

HRESULT IDirect3DDevice9Proxy::EvictManagedResources()
{
    return m_device->EvictManagedResources();
}

//-----------------------------------------------------------------------------

HRESULT IDirect3DDevice9Proxy::GetDirect3D( IDirect3D9 **ppD3D9)
{
    // HOOK return our direct 3D proxy instead of the real one
    *ppD3D9 = m_direct3D;
    return D3D_OK;
}

//-----------------------------------------------------------------------------

HRESULT IDirect3DDevice9Proxy::GetDeviceCaps( D3DCAPS9 *pCaps)
{
    return m_device->GetDeviceCaps( pCaps );
}

//-----------------------------------------------------------------------------

HRESULT IDirect3DDevice9Proxy::GetDisplayMode(
    UINT iSwapChain,
    D3DDISPLAYMODE *pMode
) {
    return m_device->GetDisplayMode( iSwapChain, pMode );
}

//-----------------------------------------------------------------------------

HRESULT IDirect3DDevice9Proxy::GetCreationParameters(
    D3DDEVICE_CREATION_PARAMETERS *pParameters
) {
    return m_device->GetCreationParameters( pParameters );
}

//-----------------------------------------------------------------------------

HRESULT IDirect3DDevice9Proxy::SetCursorProperties(
    UINT XHotSpot,
    UINT YHotSpot,
    IDirect3DSurface9 *pCursorBitmap
) {
    return m_device->SetCursorProperties( XHotSpot, YHotSpot, pCursorBitmap );
}

//-----------------------------------------------------------------------------

void IDirect3DDevice9Proxy::SetCursorPosition( int X, int Y, DWORD Flags )
{
    return m_device->SetCursorPosition( X, Y, Flags );
}

//-----------------------------------------------------------------------------

BOOL IDirect3DDevice9Proxy::ShowCursor( BOOL bShow )
{
    return m_device->ShowCursor( bShow );
}

//-----------------------------------------------------------------------------

HRESULT IDirect3DDevice9Proxy::CreateAdditionalSwapChain(
    D3DPRESENT_PARAMETERS *pPresentationParameters,
    IDirect3DSwapChain9 **pSwapChain
) {
    return m_device->CreateAdditionalSwapChain(
        pPresentationParameters, pSwapChain
    );
}

//-----------------------------------------------------------------------------

HRESULT IDirect3DDevice9Proxy::GetSwapChain( 
    UINT iSwapChain,
    IDirect3DSwapChain9 **pSwapChain
) {
    return m_device->GetSwapChain( iSwapChain, pSwapChain );
}

//-----------------------------------------------------------------------------

UINT IDirect3DDevice9Proxy::GetNumberOfSwapChains()
{
    return m_device->GetNumberOfSwapChains();
}

//-----------------------------------------------------------------------------

HRESULT IDirect3DDevice9Proxy::Reset(
    D3DPRESENT_PARAMETERS *pPresentParam
) {

    Log::print("IDirect3DDevice9::Reset: ")
        << pPresentParam->BackBufferWidth << 'x' << pPresentParam->BackBufferHeight << ','
        << "backBuffers=" << pPresentParam->BackBufferCount << ','
        << "windowed=" << pPresentParam->Windowed << endl;

    if ( pPresentParam->Windowed == FALSE ) {
        Log::print("Forcing windowed mode\n");
        // enforce windowed mode
        pPresentParam->Windowed = TRUE;
        pPresentParam->FullScreen_RefreshRateInHz = 0;
    }

    return m_device->Reset( pPresentParam );
}

//-----------------------------------------------------------------------------

HRESULT IDirect3DDevice9Proxy::Present(
    CONST RECT *pSourceRect,
    CONST RECT *pDestRect,
    HWND hDestWindowOverride,
    CONST RGNDATA *pDirtyRegion
) {
    const bool passThrough = Settings::get().passThrough;

    if ( !passThrough ) {
        m_quad.onPrePresentDX(
            pSourceRect,
            pDestRect,
            hDestWindowOverride,
            pDirtyRegion
        );
    }
    
    HRESULT result = D3D_OK;

    if ( passThrough ) {
        result = m_device->Present(
            pSourceRect,
            pDestRect,
            hDestWindowOverride,
            pDirtyRegion
        );
    } else {
        // Ignore calls to Present() and pretend that it succeeded, to
        // avoid flicker when we make the GL window a child of the original
        // application source window
    }

    //Log::print() << "Present("
    //  << pSourceRect << ','
    //  << pDestRect << ','
    //  << hDestWindowOverride << ','
    //  << pDirtyRegion << ") = " << result << std::endl;

    if ( !passThrough ) {
        m_quad.onPostPresentDX();
    }

    return result;
}

//-----------------------------------------------------------------------------

HRESULT IDirect3DDevice9Proxy::GetBackBuffer(
    UINT iSwapChain,
    UINT iBackBuffer,
    D3DBACKBUFFER_TYPE Type,
    IDirect3DSurface9 **ppBackBuffer
) {
    return m_device->GetBackBuffer(
        iSwapChain,
        iBackBuffer,
        Type,
        ppBackBuffer
    );
}

//-----------------------------------------------------------------------------

HRESULT IDirect3DDevice9Proxy::GetRasterStatus(
    UINT iSwapChain,
    D3DRASTER_STATUS *pRasterStatus
) {
    return m_device->GetRasterStatus( iSwapChain, pRasterStatus );
}

//-----------------------------------------------------------------------------

HRESULT IDirect3DDevice9Proxy::SetDialogBoxMode( BOOL bEnableDialogs )
{
    return m_device->SetDialogBoxMode( bEnableDialogs );
}

//-----------------------------------------------------------------------------

void IDirect3DDevice9Proxy::SetGammaRamp(
    UINT iSwapChain,
    DWORD Flags,
    CONST D3DGAMMARAMP *pRamp
) {
    return m_device->SetGammaRamp( iSwapChain, Flags, pRamp );
}

//-----------------------------------------------------------------------------

void IDirect3DDevice9Proxy::GetGammaRamp(
    UINT iSwapChain,
    D3DGAMMARAMP *pRamp
) {
    return m_device->GetGammaRamp( iSwapChain, pRamp );
}

//-----------------------------------------------------------------------------

HRESULT IDirect3DDevice9Proxy::CreateTexture(
    UINT Width,
    UINT Height,
    UINT Levels,
    DWORD Usage,
    D3DFORMAT Format,
    D3DPOOL Pool,
    IDirect3DTexture9 **ppTexture,
    HANDLE *pSharedHandle
) {
    if ( Settings::get().forceDirect3D9Ex && (Pool == D3DPOOL_MANAGED) ) {
        // replace D3DPOOL_MANAGED with D3DPOOL_DEFAULT
        Pool = D3DPOOL_DEFAULT;
        // set D3DUSAGE_DYNAMIC so that texture can be locked
        Usage |= D3DUSAGE_DYNAMIC;
    }

    HRESULT result = m_device->CreateTexture(
        Width, Height,
        Levels,
        Usage,
        Format,
        Pool,
        ppTexture,
        pSharedHandle
    );

    Log::print() << "CreateTexture("
        << Width << ',' << Height << ','
        << Usage << ','
        << Format << ','
        << Pool << ','
        << ppTexture << ','
        << pSharedHandle << ") = " << result << std::endl;

    return result;
}

//-----------------------------------------------------------------------------

HRESULT IDirect3DDevice9Proxy::CreateVolumeTexture(
    UINT Width,
    UINT Height,
    UINT Depth,
    UINT Levels,
    DWORD Usage,
    D3DFORMAT Format,
    D3DPOOL Pool,
    IDirect3DVolumeTexture9 **ppVolumeTexture,
    HANDLE *pSharedHandle
) {
    if ( Settings::get().forceDirect3D9Ex && (Pool == D3DPOOL_MANAGED) )
        Pool = D3DPOOL_DEFAULT;

    return m_device->CreateVolumeTexture(
        Width, Height, Depth,
        Levels,
        Usage,
        Format,
        Pool,
        ppVolumeTexture,
        pSharedHandle
    );
}

//-----------------------------------------------------------------------------

HRESULT IDirect3DDevice9Proxy::CreateCubeTexture(
    UINT EdgeLength,
    UINT Levels,
    DWORD Usage,
    D3DFORMAT Format,
    D3DPOOL Pool,
    IDirect3DCubeTexture9 **ppCubeTexture,
    HANDLE *pSharedHandle
) {
    if ( Settings::get().forceDirect3D9Ex && (Pool == D3DPOOL_MANAGED) )
        Pool = D3DPOOL_DEFAULT;

    return m_device->CreateCubeTexture(
        EdgeLength,
        Levels,
        Usage,
        Format,
        Pool,
        ppCubeTexture,
        pSharedHandle
    );
}

//-----------------------------------------------------------------------------

HRESULT IDirect3DDevice9Proxy::CreateVertexBuffer(
    UINT Length,
    DWORD Usage,
    DWORD FVF,
    D3DPOOL Pool,
    IDirect3DVertexBuffer9 **ppVertexBuffer,
    HANDLE *pSharedHandle
) {
    D3DPOOL newPool = Pool;
    if ( Settings::get().forceDirect3D9Ex && (Pool == D3DPOOL_MANAGED) )
        newPool = D3DPOOL_DEFAULT;

    HRESULT result = m_device->CreateVertexBuffer(
        Length,
        Usage,
        FVF,
        newPool,
        ppVertexBuffer,
        pSharedHandle
    );

    string poolChanged( (newPool != Pool) ? ":changed" : "" );

    Log::print() << "CreateVertexBuffer("
        << Length << ','
        << Usage << "=(" << D3DUSAGEtoString( Usage ) << "),"
        << FVF << "=(" << D3DFVFtoString( FVF ) << "),"
        << Pool << "=(" << D3DPOOLtoString( Pool ) << poolChanged << "),"
        << ppVertexBuffer
        << '[' << ((ppVertexBuffer != 0) ? *ppVertexBuffer : 0) << "],"
        << pSharedHandle << ") = " << result << std::endl;

    return result;
}

//-----------------------------------------------------------------------------

HRESULT IDirect3DDevice9Proxy::CreateIndexBuffer(
    UINT Length,
    DWORD Usage,
    D3DFORMAT Format,
    D3DPOOL Pool,
    IDirect3DIndexBuffer9 **ppIndexBuffer,
    HANDLE *pSharedHandle
) {
    D3DPOOL newPool = Pool;
    if ( Settings::get().forceDirect3D9Ex && (Pool == D3DPOOL_MANAGED) )
        newPool = D3DPOOL_DEFAULT;

    HRESULT result = m_device->CreateIndexBuffer(
        Length,
        Usage,
        Format,
        newPool,
        ppIndexBuffer,
        pSharedHandle
    );

    string poolChanged( (newPool != Pool) ? ":changed" : "" );

    Log::print() << "CreateIndexBuffer("
        << Length << ','
        << Usage << "=(" << D3DUSAGEtoString( Usage ) << "),"
        << Format << "=(" << D3DFORMATtoString( Format ) << "),"
        << Pool << "=(" << D3DPOOLtoString( Pool ) << poolChanged << "),"
        << ppIndexBuffer
        << '[' << ((ppIndexBuffer != 0) ? *ppIndexBuffer : 0) << "],"
        << pSharedHandle << ") = " << result << std::endl;

    return result;
}

//-----------------------------------------------------------------------------

HRESULT IDirect3DDevice9Proxy::CreateRenderTarget(
    UINT Width,
    UINT Height,
    D3DFORMAT Format,
    D3DMULTISAMPLE_TYPE MultiSample,
    DWORD MultisampleQuality,
    BOOL Lockable,
    IDirect3DSurface9 **ppSurface,
    HANDLE *pSharedHandle
) {
    //Log::print() << "CreateRenderTarget("
    //    << Width << ','
    //    << Height << ','
    //    << Format << "=(" << D3DFORMATtoString( Format ) << "),"
    //    << MultiSample << "=(" << D3DMULTISAMPLE_TYPEtoString( MultiSample ) <<"),"
    //    << MultisampleQuality << ','
    //    << Lockable << ','
    //    << ppSurface << ','
    //    << pSharedHandle << ")\n";

    return m_device->CreateRenderTarget(
        Width, Height,
        Format,
        MultiSample,
        MultisampleQuality,
        Lockable,
        ppSurface,
        pSharedHandle
    );
}

//-----------------------------------------------------------------------------

HRESULT IDirect3DDevice9Proxy::CreateDepthStencilSurface(
    UINT Width,
    UINT Height,
    D3DFORMAT Format,
    D3DMULTISAMPLE_TYPE MultiSample,
    DWORD MultisampleQuality,
    BOOL Discard,
    IDirect3DSurface9 **ppSurface,
    HANDLE *pSharedHandle
) {
    return m_device->CreateDepthStencilSurface(
        Width, Height,
        Format,
        MultiSample,
        MultisampleQuality,
        Discard,
        ppSurface,
        pSharedHandle
    );
}

//-----------------------------------------------------------------------------

HRESULT IDirect3DDevice9Proxy::UpdateSurface(
    IDirect3DSurface9 *pSourceSurface,
    CONST RECT *pSourceRect,
    IDirect3DSurface9 *pDestinationSurface,
    CONST POINT *pDestPoint
) {
    return m_device->UpdateSurface(
        pSourceSurface,
        pSourceRect,
        pDestinationSurface,
        pDestPoint
    );
}

//-----------------------------------------------------------------------------

HRESULT IDirect3DDevice9Proxy::UpdateTexture(
    IDirect3DBaseTexture9 *pSourceTexture,
    IDirect3DBaseTexture9 *pDestinationTexture
) {
    return m_device->UpdateTexture( pSourceTexture, pDestinationTexture );
}

//-----------------------------------------------------------------------------

HRESULT IDirect3DDevice9Proxy::GetRenderTargetData(
    IDirect3DSurface9 *pRenderTarget,
    IDirect3DSurface9 *pDestSurface
) {
    return m_device->GetRenderTargetData( pRenderTarget, pDestSurface );
}

//-----------------------------------------------------------------------------

HRESULT IDirect3DDevice9Proxy::GetFrontBufferData(
    UINT iSwapChain,
    IDirect3DSurface9 *pDestSurface
) {
    return m_device->GetFrontBufferData( iSwapChain, pDestSurface );
}

//-----------------------------------------------------------------------------

HRESULT IDirect3DDevice9Proxy::StretchRect(
    IDirect3DSurface9 *pSourceSurface,
    CONST RECT *pSourceRect,
    IDirect3DSurface9 *pDestSurface,
    CONST RECT *pDestRect,
    D3DTEXTUREFILTERTYPE Filter
) {
    return m_device->StretchRect(
        pSourceSurface,
        pSourceRect,
        pDestSurface,
        pDestRect,
        Filter
    );
}

//-----------------------------------------------------------------------------

HRESULT IDirect3DDevice9Proxy::ColorFill(
    IDirect3DSurface9 *pSurface,
    CONST RECT *pRect,
    D3DCOLOR color
) {
    return m_device->ColorFill( pSurface, pRect, color );
}

//-----------------------------------------------------------------------------

HRESULT IDirect3DDevice9Proxy::CreateOffscreenPlainSurface(
    UINT Width,
    UINT Height,
    D3DFORMAT Format,
    D3DPOOL Pool,
    IDirect3DSurface9 **ppSurface,
    HANDLE *pSharedHandle
) {
    //Log::print() << "CreateOffscreenPlainSurface("
    //    << Width << ','
    //    << Height << ','
    //    << Format << "=(" << D3DFORMATtoString( Format ) << "),"
    //    << Pool << "=(" << D3DPOOLtoString( Pool ) << "),"
    //    << ppSurface << ','
    //    << pSharedHandle << ")\n";

    if ( Settings::get().forceDirect3D9Ex && (Pool == D3DPOOL_MANAGED) )
        Pool = D3DPOOL_DEFAULT;

    return m_device->CreateOffscreenPlainSurface(
        Width, Height,
        Format,
        Pool,
        ppSurface,
        pSharedHandle
    );
}

//-----------------------------------------------------------------------------

HRESULT IDirect3DDevice9Proxy::SetRenderTarget(
    DWORD RenderTargetIndex,
    IDirect3DSurface9 *pRenderTarget
) {
    return m_device->SetRenderTarget( RenderTargetIndex, pRenderTarget );
}

//-----------------------------------------------------------------------------

HRESULT IDirect3DDevice9Proxy::GetRenderTarget(
    DWORD RenderTargetIndex,
    IDirect3DSurface9 **ppRenderTarget
) {
    return m_device->GetRenderTarget( RenderTargetIndex, ppRenderTarget );
}

//-----------------------------------------------------------------------------

HRESULT IDirect3DDevice9Proxy::SetDepthStencilSurface(
    IDirect3DSurface9 *pNewZStencil
) {
    return m_device->SetDepthStencilSurface( pNewZStencil );
}

//-----------------------------------------------------------------------------

HRESULT IDirect3DDevice9Proxy::GetDepthStencilSurface(
    IDirect3DSurface9 **ppZStencilSurface
) {
    return m_device->GetDepthStencilSurface( ppZStencilSurface );
}

//-----------------------------------------------------------------------------

HRESULT IDirect3DDevice9Proxy::BeginScene()
{
    return m_device->BeginScene();
}

//-----------------------------------------------------------------------------

HRESULT IDirect3DDevice9Proxy::EndScene()
{
    return m_device->EndScene();
}

//-----------------------------------------------------------------------------

HRESULT IDirect3DDevice9Proxy::Clear(
    DWORD Count,
    CONST D3DRECT *pRects,
    DWORD Flags,
    D3DCOLOR Color,
    float Z,
    DWORD Stencil
) {
    //Log::print() << "IDirect3DDevice9::Clear(...)\n";

    if ( !Settings::get().passThrough )
        m_quad.onPreClearDX( Count, pRects, Flags, Color, Z, Stencil );
    
    HRESULT result = m_device->Clear(
        Count, pRects, Flags, Color, Z, Stencil
    );
    
    if ( !Settings::get().passThrough )
        m_quad.onPostClearDX();

    return result;
}

//-----------------------------------------------------------------------------

HRESULT IDirect3DDevice9Proxy::SetTransform(
    D3DTRANSFORMSTATETYPE State,
    CONST D3DMATRIX *pMatrix
) {
    return m_device->SetTransform( State, pMatrix );
}

//-----------------------------------------------------------------------------

HRESULT IDirect3DDevice9Proxy::GetTransform(
    D3DTRANSFORMSTATETYPE State,
    D3DMATRIX *pMatrix
) {
    return m_device->GetTransform( State, pMatrix );
}

//-----------------------------------------------------------------------------

HRESULT IDirect3DDevice9Proxy::MultiplyTransform(
    D3DTRANSFORMSTATETYPE State,
    CONST D3DMATRIX *Matrix
) {
    return m_device->MultiplyTransform( State, Matrix );
}

//-----------------------------------------------------------------------------

HRESULT IDirect3DDevice9Proxy::SetViewport( CONST D3DVIEWPORT9 *pViewport )
{
    //Log::print() << "SetViewport: "
    //    << pViewport->X << ','
    //    << pViewport->Y << ' '
    //    << pViewport->Width  << 'x'
    //    << pViewport->Height << ' '
    //    << pViewport->MinZ << '..'
    //    << pViewport->MaxZ << endl;

    return m_device->SetViewport( pViewport );
}

//-----------------------------------------------------------------------------

HRESULT IDirect3DDevice9Proxy::GetViewport( D3DVIEWPORT9 *pViewport )
{
    return m_device->GetViewport( pViewport );
}

//-----------------------------------------------------------------------------

HRESULT IDirect3DDevice9Proxy::SetMaterial( CONST D3DMATERIAL9 *pMaterial )
{
    return m_device->SetMaterial( pMaterial );
}

//-----------------------------------------------------------------------------

HRESULT IDirect3DDevice9Proxy::GetMaterial( D3DMATERIAL9 *pMaterial )
{
    return m_device->GetMaterial( pMaterial );
}

//-----------------------------------------------------------------------------

HRESULT IDirect3DDevice9Proxy::SetLight( DWORD Index, CONST D3DLIGHT9 *pLight )
{
    return m_device->SetLight( Index, pLight );
}

//-----------------------------------------------------------------------------

HRESULT IDirect3DDevice9Proxy::GetLight( DWORD Index, D3DLIGHT9 *pLight )
{
    return m_device->GetLight( Index, pLight );
}

//-----------------------------------------------------------------------------

HRESULT IDirect3DDevice9Proxy::LightEnable( DWORD Index, BOOL Enable )
{
    return m_device->LightEnable( Index, Enable );
}

//-----------------------------------------------------------------------------

HRESULT IDirect3DDevice9Proxy::GetLightEnable( DWORD Index, BOOL *pEnable )
{
    return m_device->GetLightEnable( Index, pEnable );
}

//-----------------------------------------------------------------------------

HRESULT IDirect3DDevice9Proxy::SetClipPlane( DWORD Index, CONST float *pPlane )
{
    return m_device->SetClipPlane( Index, pPlane );
}

//-----------------------------------------------------------------------------

HRESULT IDirect3DDevice9Proxy::GetClipPlane( DWORD Index, float *pPlane )
{
    return m_device->GetClipPlane( Index, pPlane );
}

//-----------------------------------------------------------------------------

HRESULT IDirect3DDevice9Proxy::SetRenderState(
    D3DRENDERSTATETYPE State,
    DWORD Value
) {
    return m_device->SetRenderState( State, Value );
}

//-----------------------------------------------------------------------------

HRESULT IDirect3DDevice9Proxy::GetRenderState(
    D3DRENDERSTATETYPE State,
    DWORD *pValue
) {
    return m_device->GetRenderState( State, pValue );
}

//-----------------------------------------------------------------------------

HRESULT IDirect3DDevice9Proxy::CreateStateBlock(
    D3DSTATEBLOCKTYPE Type,
    IDirect3DStateBlock9 **ppSB
) {
    return m_device->CreateStateBlock( Type, ppSB );
}

//-----------------------------------------------------------------------------

HRESULT IDirect3DDevice9Proxy::BeginStateBlock()
{
    return m_device->BeginStateBlock();
}

//-----------------------------------------------------------------------------

HRESULT IDirect3DDevice9Proxy::EndStateBlock( IDirect3DStateBlock9 **ppSB )
{
    return m_device->EndStateBlock( ppSB );
}

//-----------------------------------------------------------------------------

HRESULT IDirect3DDevice9Proxy::SetClipStatus(
    CONST D3DCLIPSTATUS9 *pClipStatus
) {
    return m_device->SetClipStatus( pClipStatus );
}

//-----------------------------------------------------------------------------

HRESULT IDirect3DDevice9Proxy::GetClipStatus( D3DCLIPSTATUS9 *pClipStatus )
{
    return m_device->GetClipStatus( pClipStatus );
}

//-----------------------------------------------------------------------------

HRESULT IDirect3DDevice9Proxy::GetTexture(
    DWORD Stage,
    IDirect3DBaseTexture9 **ppTexture
) {
    return m_device->GetTexture( Stage, ppTexture );
}

//-----------------------------------------------------------------------------

HRESULT IDirect3DDevice9Proxy::SetTexture(
    DWORD Stage,
    IDirect3DBaseTexture9 *pTexture
) {
    return m_device->SetTexture( Stage, pTexture );
}

//-----------------------------------------------------------------------------

HRESULT IDirect3DDevice9Proxy::GetTextureStageState(
    DWORD Stage,
    D3DTEXTURESTAGESTATETYPE Type,
    DWORD *pValue
) {
    return m_device->GetTextureStageState( Stage, Type, pValue );
}

//-----------------------------------------------------------------------------

HRESULT IDirect3DDevice9Proxy::SetTextureStageState(
    DWORD Stage,
    D3DTEXTURESTAGESTATETYPE Type,
    DWORD Value
) {
    return m_device->SetTextureStageState( Stage, Type, Value );
}

//-----------------------------------------------------------------------------

HRESULT IDirect3DDevice9Proxy::GetSamplerState(
    DWORD Sampler,
    D3DSAMPLERSTATETYPE Type,
    DWORD *pValue
) {
    return m_device->GetSamplerState( Sampler, Type, pValue );
}

//-----------------------------------------------------------------------------

HRESULT IDirect3DDevice9Proxy::SetSamplerState(
    DWORD Sampler,
    D3DSAMPLERSTATETYPE Type,
    DWORD Value
) {
    return m_device->SetSamplerState( Sampler, Type, Value );
}

//-----------------------------------------------------------------------------

HRESULT IDirect3DDevice9Proxy::ValidateDevice( DWORD *pNumPasses )
{
    return m_device->ValidateDevice( pNumPasses );
}

//-----------------------------------------------------------------------------

HRESULT IDirect3DDevice9Proxy::SetPaletteEntries(
    UINT PaletteNumber,
    CONST PALETTEENTRY *pEntries
) {
    return m_device->SetPaletteEntries( PaletteNumber, pEntries );
}

//-----------------------------------------------------------------------------

HRESULT IDirect3DDevice9Proxy::GetPaletteEntries(
    UINT PaletteNumber,
    PALETTEENTRY *pEntries
) {
    return m_device->GetPaletteEntries( PaletteNumber, pEntries );
}

//-----------------------------------------------------------------------------

HRESULT IDirect3DDevice9Proxy::SetCurrentTexturePalette( UINT PaletteNumber )
{
    return m_device->SetCurrentTexturePalette( PaletteNumber );
}

//-----------------------------------------------------------------------------

HRESULT IDirect3DDevice9Proxy::GetCurrentTexturePalette( UINT *PaletteNumber )
{
    return m_device->GetCurrentTexturePalette( PaletteNumber );
}

//-----------------------------------------------------------------------------

HRESULT IDirect3DDevice9Proxy::SetScissorRect( CONST RECT *pRect )
{
    return m_device->SetScissorRect( pRect );
}

//-----------------------------------------------------------------------------

HRESULT IDirect3DDevice9Proxy::GetScissorRect( RECT *pRect )
{
    return m_device->GetScissorRect( pRect );
}

//-----------------------------------------------------------------------------

HRESULT IDirect3DDevice9Proxy::SetSoftwareVertexProcessing( BOOL bSoftware )
{
    return m_device->SetSoftwareVertexProcessing( bSoftware );
}

//-----------------------------------------------------------------------------

BOOL IDirect3DDevice9Proxy::GetSoftwareVertexProcessing()
{
    return m_device->GetSoftwareVertexProcessing();
}

//-----------------------------------------------------------------------------

HRESULT IDirect3DDevice9Proxy::SetNPatchMode( float nSegments )
{
    return m_device->SetNPatchMode( nSegments );
}

//-----------------------------------------------------------------------------

float IDirect3DDevice9Proxy::GetNPatchMode()
{
    return m_device->GetNPatchMode();
}


//-----------------------------------------------------------------------------

HRESULT IDirect3DDevice9Proxy::DrawPrimitive(
    D3DPRIMITIVETYPE PrimitiveType,
    UINT StartVertex,
    UINT PrimitiveCount
) {
    return m_device->DrawPrimitive(
        PrimitiveType, StartVertex, PrimitiveCount
    );
}


//-----------------------------------------------------------------------------

HRESULT IDirect3DDevice9Proxy::DrawIndexedPrimitive(
    D3DPRIMITIVETYPE PrimitiveType,
    INT BaseVertexIndex,
    UINT MinVertexIndex,
    UINT NumVertices,
    UINT startIndex,
    UINT primCount
) {
    return m_device->DrawIndexedPrimitive(
        PrimitiveType,
        BaseVertexIndex,
        MinVertexIndex,
        NumVertices,
        startIndex,
        primCount
    );
}


//-----------------------------------------------------------------------------

HRESULT IDirect3DDevice9Proxy::DrawPrimitiveUP(
    D3DPRIMITIVETYPE PrimitiveType,
    UINT PrimitiveCount,
    CONST void *pVertexStreamZeroData,
    UINT VertexStreamZeroStride
) {
    return m_device->DrawPrimitiveUP(
        PrimitiveType,
        PrimitiveCount,
        pVertexStreamZeroData,
        VertexStreamZeroStride
    );
}


//-----------------------------------------------------------------------------

HRESULT IDirect3DDevice9Proxy::DrawIndexedPrimitiveUP(
    D3DPRIMITIVETYPE PrimitiveType,
    UINT MinVertexIndex,
    UINT NumVertices,
    UINT PrimitiveCount,
    CONST void *pIndexData,
    D3DFORMAT IndexDataFormat,
    CONST void *pVertexStreamZeroData,
    UINT VertexStreamZeroStride
) {
    return m_device->DrawIndexedPrimitiveUP(
        PrimitiveType,
        MinVertexIndex,
        NumVertices,
        PrimitiveCount,
        pIndexData,
        IndexDataFormat,
        pVertexStreamZeroData,
        VertexStreamZeroStride
    );
}


//-----------------------------------------------------------------------------

HRESULT IDirect3DDevice9Proxy::ProcessVertices(
    UINT SrcStartIndex,
    UINT DestIndex,
    UINT VertexCount,
    IDirect3DVertexBuffer9 *pDestBuffer,
    IDirect3DVertexDeclaration9 *pVertexDecl,
    DWORD Flags
) {
    return m_device->ProcessVertices(
        SrcStartIndex,
        DestIndex,
        VertexCount,
        pDestBuffer,
        pVertexDecl,
        Flags
    );
}


//-----------------------------------------------------------------------------

HRESULT IDirect3DDevice9Proxy::CreateVertexDeclaration(
    CONST D3DVERTEXELEMENT9 *pVertexElements,
    IDirect3DVertexDeclaration9 **ppDecl
) {
    return m_device->CreateVertexDeclaration( pVertexElements, ppDecl );
}


//-----------------------------------------------------------------------------

HRESULT IDirect3DDevice9Proxy::SetVertexDeclaration(
    IDirect3DVertexDeclaration9 *pDecl
) {
    return m_device->SetVertexDeclaration( pDecl );
}


//-----------------------------------------------------------------------------

HRESULT IDirect3DDevice9Proxy::GetVertexDeclaration(
    IDirect3DVertexDeclaration9 **ppDecl
) {
    return m_device->GetVertexDeclaration( ppDecl );
}


//-----------------------------------------------------------------------------

HRESULT IDirect3DDevice9Proxy::SetFVF( DWORD FVF )
{
    return m_device->SetFVF( FVF );
}


//-----------------------------------------------------------------------------

HRESULT IDirect3DDevice9Proxy::GetFVF( DWORD *pFVF )
{
    return m_device->GetFVF( pFVF );
}


//-----------------------------------------------------------------------------

HRESULT IDirect3DDevice9Proxy::CreateVertexShader(
    CONST DWORD *pFunction,
    IDirect3DVertexShader9 **ppShader
) {
    return m_device->CreateVertexShader( pFunction, ppShader );
}


//-----------------------------------------------------------------------------

HRESULT IDirect3DDevice9Proxy::SetVertexShader(
    IDirect3DVertexShader9 *pShader
) {
    return m_device->SetVertexShader( pShader );
}


//-----------------------------------------------------------------------------

HRESULT IDirect3DDevice9Proxy::GetVertexShader(
    IDirect3DVertexShader9 **ppShader
) {
    return m_device->GetVertexShader( ppShader );
}


//-----------------------------------------------------------------------------

HRESULT IDirect3DDevice9Proxy::SetVertexShaderConstantF(
    UINT StartRegister,
    CONST float *pConstantData,
    UINT Vector4fCount
) {
    return m_device->SetVertexShaderConstantF(
        StartRegister,
        pConstantData,
        Vector4fCount
    );
}


//-----------------------------------------------------------------------------

HRESULT IDirect3DDevice9Proxy::GetVertexShaderConstantF(
    UINT StartRegister,
    float *pConstantData,
    UINT Vector4fCount
) {
    return m_device->GetVertexShaderConstantF(
        StartRegister,
        pConstantData,
        Vector4fCount
    );
}


//-----------------------------------------------------------------------------

HRESULT IDirect3DDevice9Proxy::SetVertexShaderConstantI(
    UINT StartRegister,
    CONST int *pConstantData,
    UINT Vector4iCount
) {
    return m_device->SetVertexShaderConstantI(
        StartRegister,
        pConstantData,
        Vector4iCount
    );
}


//-----------------------------------------------------------------------------

HRESULT IDirect3DDevice9Proxy::GetVertexShaderConstantI(
    UINT StartRegister,
    int *pConstantData,
    UINT Vector4iCount
) {
    return m_device->GetVertexShaderConstantI(
        StartRegister,
        pConstantData,
        Vector4iCount
    );
}


//-----------------------------------------------------------------------------

HRESULT IDirect3DDevice9Proxy::SetVertexShaderConstantB(
    UINT StartRegister,
    CONST BOOL *pConstantData,
    UINT  BoolCount
) {
    return m_device->SetVertexShaderConstantB(
        StartRegister,
        pConstantData,
        BoolCount
    );
}


//-----------------------------------------------------------------------------

HRESULT IDirect3DDevice9Proxy::GetVertexShaderConstantB(
    UINT StartRegister,
    BOOL *pConstantData,
    UINT BoolCount
) {
    return m_device->GetVertexShaderConstantB(
        StartRegister,
        pConstantData,
        BoolCount
    );
}


//-----------------------------------------------------------------------------

HRESULT IDirect3DDevice9Proxy::SetStreamSource(
    UINT StreamNumber,
    IDirect3DVertexBuffer9 *pStreamData,
    UINT OffsetInBytes,
    UINT Stride
) {
    return m_device->SetStreamSource(
        StreamNumber,
        pStreamData,
        OffsetInBytes,
        Stride
    );
}


//-----------------------------------------------------------------------------

HRESULT IDirect3DDevice9Proxy::GetStreamSource(
    UINT StreamNumber,
    IDirect3DVertexBuffer9 **ppStreamData,
    UINT *pOffsetInBytes,
    UINT *pStride
) {
    return m_device->GetStreamSource(
        StreamNumber,
        ppStreamData,
        pOffsetInBytes,
        pStride
    );
}


//-----------------------------------------------------------------------------

HRESULT IDirect3DDevice9Proxy::SetStreamSourceFreq(
    UINT StreamNumber,
    UINT Setting
) {
    return m_device->SetStreamSourceFreq( StreamNumber, Setting );
}


//-----------------------------------------------------------------------------

HRESULT IDirect3DDevice9Proxy::GetStreamSourceFreq(
    UINT StreamNumber,
    UINT *pSetting
) {
    return m_device->GetStreamSourceFreq( StreamNumber, pSetting );
}


//-----------------------------------------------------------------------------

HRESULT IDirect3DDevice9Proxy::SetIndices(
    IDirect3DIndexBuffer9 *pIndexData
) {
    return m_device->SetIndices( pIndexData );
}


//-----------------------------------------------------------------------------

HRESULT IDirect3DDevice9Proxy::GetIndices(
    IDirect3DIndexBuffer9 **ppIndexData
) {
    return m_device->GetIndices( ppIndexData );
}

//-----------------------------------------------------------------------------

HRESULT IDirect3DDevice9Proxy::CreatePixelShader(
    CONST DWORD *pFunction,
    IDirect3DPixelShader9 **ppShader
) {
    return m_device->CreatePixelShader( pFunction, ppShader );
}


//-----------------------------------------------------------------------------

HRESULT IDirect3DDevice9Proxy::SetPixelShader( IDirect3DPixelShader9 *pShader )
{
    return m_device->SetPixelShader( pShader );
}


//-----------------------------------------------------------------------------

HRESULT IDirect3DDevice9Proxy::GetPixelShader(
    IDirect3DPixelShader9 **ppShader
) {
    return m_device->GetPixelShader( ppShader );
}


//-----------------------------------------------------------------------------

HRESULT IDirect3DDevice9Proxy::SetPixelShaderConstantF(
    UINT StartRegister,
    CONST float *pConstantData,
    UINT Vector4fCount
) {
    return m_device->SetPixelShaderConstantF(
        StartRegister,
        pConstantData,
        Vector4fCount
    );
}


//-----------------------------------------------------------------------------

HRESULT IDirect3DDevice9Proxy::GetPixelShaderConstantF(
    UINT StartRegister,
    float *pConstantData,
    UINT Vector4fCount
) {
    return m_device->GetPixelShaderConstantF(
        StartRegister,
        pConstantData,
        Vector4fCount
    );
}


//-----------------------------------------------------------------------------

HRESULT IDirect3DDevice9Proxy::SetPixelShaderConstantI(
    UINT StartRegister,
    CONST int *pConstantData,
    UINT Vector4iCount
) {
    return m_device->SetPixelShaderConstantI(
        StartRegister,
        pConstantData,
        Vector4iCount
    );
}


//-----------------------------------------------------------------------------

HRESULT IDirect3DDevice9Proxy::GetPixelShaderConstantI(
    UINT StartRegister,
    int *pConstantData,
    UINT Vector4iCount
) {
    return m_device->GetPixelShaderConstantI(
        StartRegister,
        pConstantData,
        Vector4iCount
    );
}


//-----------------------------------------------------------------------------

HRESULT IDirect3DDevice9Proxy::SetPixelShaderConstantB(
    UINT StartRegister,
    CONST BOOL *pConstantData,
    UINT  BoolCount
) {
    return m_device->SetPixelShaderConstantB(
        StartRegister,
        pConstantData,
        BoolCount
    );
}


//-----------------------------------------------------------------------------

HRESULT IDirect3DDevice9Proxy::GetPixelShaderConstantB(
    UINT StartRegister,
    BOOL *pConstantData,
    UINT BoolCount
) {
    return m_device->GetPixelShaderConstantB(
        StartRegister,
        pConstantData,
        BoolCount
    );
}


//-----------------------------------------------------------------------------

HRESULT IDirect3DDevice9Proxy::DrawRectPatch(
    UINT Handle,
    CONST float *pNumSegs,
    CONST D3DRECTPATCH_INFO *pRectPatchInfo
) {
    return m_device->DrawRectPatch(
        Handle,
        pNumSegs,
        pRectPatchInfo
    );
}


//-----------------------------------------------------------------------------

HRESULT IDirect3DDevice9Proxy::DrawTriPatch(
    UINT Handle,
    CONST float *pNumSegs,
    CONST D3DTRIPATCH_INFO *pTriPatchInfo
) {
    return m_device->DrawTriPatch( Handle, pNumSegs, pTriPatchInfo );
}


//-----------------------------------------------------------------------------

HRESULT IDirect3DDevice9Proxy::DeletePatch( UINT Handle )
{
    return m_device->DeletePatch( Handle );
}


//-----------------------------------------------------------------------------

HRESULT IDirect3DDevice9Proxy::CreateQuery(
    D3DQUERYTYPE Type,
    IDirect3DQuery9 **ppQuery
) {
    return m_device->CreateQuery( Type, ppQuery );
}


//-----------------------------------------------------------------------------

