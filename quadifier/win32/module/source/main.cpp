#include <windows.h>
#include <mhook-lib/mhook.h>
#include <d3d9.h>
#include <atlconv.h>
#include "IDirect3D9Proxy.h"

#if defined(SUPPORT_D3D11)
    // only include these files if configured to build in D3D11 support
    //#include <d3d10_1.h>
    //#include <d3d10.h>
    #include <d3d11.h>
    #include <DXGI.h>
    #include "ID3D11DeviceProxy.h"
    #include "ID3D11DeviceContextProxy.h"
    #include "DXGISwapChainProxy.h"
    #include "DXGIFactory1Proxy.h"
#endif

#include "Log.h"
#include "Settings.h"
#include <iostream>

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

typedef FARPROC (WINAPI *PFNGetProcAddress)( HMODULE hModule, LPCSTR lpProcName );

typedef IDirect3D9 * (WINAPI *PFNDirect3DCreate9)( UINT SDKVersion );

typedef HRESULT (WINAPI *PFNDirect3DCreate9Ex)( UINT SDKVersion, IDirect3D9Ex **ppD3D );

typedef HRESULT (WINAPI *PFNCreateDXGIFactory)(REFIID riid, void **ppFactory);

typedef HRESULT (WINAPI *PFNCreateDXGIFactory1)(REFIID riid, void **ppFactory);

typedef LONG (WINAPI *PFNChangeDisplaySettingsEx)(
    _In_  LPCTSTR lpszDeviceName,
    _In_  DEVMODE *lpDevMode,
    HWND hWnd,
    _In_  DWORD dwFlags,
    _In_  LPVOID lParam
);

typedef HWND (WINAPI *PFNCreateWindowExA)(
    __in DWORD dwExStyle,
    __in_opt LPCSTR lpClassName,
    __in_opt LPCSTR lpWindowName,
    __in DWORD dwStyle,
    __in int X,
    __in int Y,
    __in int nWidth,
    __in int nHeight,
    __in_opt HWND hWndParent,
    __in_opt HMENU hMenu,
    __in_opt HINSTANCE hInstance,
    __in_opt LPVOID lpParam
);

typedef HWND (WINAPI *PFNCreateWindowExW)(
    __in DWORD dwExStyle,
    __in_opt LPCWSTR lpClassName,
    __in_opt LPCWSTR lpWindowName,
    __in DWORD dwStyle,
    __in int X,
    __in int Y,
    __in int nWidth,
    __in int nHeight,
    __in_opt HWND hWndParent,
    __in_opt HMENU hMenu,
    __in_opt HINSTANCE hInstance,
    __in_opt LPVOID lpParam
);

//----------------------------------------  -------------------------------------

PFNGetProcAddress real_GetProcAddress = reinterpret_cast<PFNGetProcAddress>(
    GetProcAddress( GetModuleHandle(L"kernel32"), "GetProcAddress" ) );

PFNDirect3DCreate9 real_Direct3DCreate9 = reinterpret_cast<PFNDirect3DCreate9>(
    GetProcAddress( GetModuleHandle(L"d3d9"), "Direct3DCreate9" ) );

PFNCreateDXGIFactory real_CreateDXGIFactory =
    reinterpret_cast<PFNCreateDXGIFactory>(
        GetProcAddress( GetModuleHandle(L"dxgi"), "CreateDXGIFactory" )
    );

PFNCreateDXGIFactory real_CreateDXGIFactory1 =
    reinterpret_cast<PFNCreateDXGIFactory1>(
        GetProcAddress( GetModuleHandle(L"dxgi"), "CreateDXGIFactory1" )
    );

PFNChangeDisplaySettingsEx real_ChangeDisplaySettingsEx =
    reinterpret_cast<PFNChangeDisplaySettingsEx>(
        GetProcAddress( GetModuleHandle(L"user32"), "ChangeDisplaySettingsExA" )
    );

PFNCreateWindowExA real_CreateWindowExA =
    reinterpret_cast<PFNCreateWindowExA>(
        GetProcAddress( GetModuleHandle(L"user32"), "CreateWindowExA" )
    );

PFNCreateWindowExW real_CreateWindowExW =
    reinterpret_cast<PFNCreateWindowExW>(
        GetProcAddress( GetModuleHandle(L"user32"), "CreateWindowExW" )
    );


//-----------------------------------------------------------------------------

#if defined(SUPPORT_D3D11)
PFN_D3D11_CREATE_DEVICE real_D3D11CreateDevice =
    reinterpret_cast<PFN_D3D11_CREATE_DEVICE>(
        GetProcAddress( GetModuleHandle(L"d3d11"), "D3D11CreateDevice" )
    );

PFN_D3D11_CREATE_DEVICE_AND_SWAP_CHAIN real_D3D11CreateDeviceAndSwapChain = 
    reinterpret_cast<PFN_D3D11_CREATE_DEVICE_AND_SWAP_CHAIN>(
        GetProcAddress( GetModuleHandle(L"d3d11"), "D3D11CreateDeviceAndSwapChain" )
    );
#endif //SUPPORT_D3D11

//-----------------------------------------------------------------------------

// force dependency on D3D9 in order that the D3D9.dll is loaded
static DWORD status = D3DPERF_GetStatus();

#if defined(SUPPORT_D3D11)
// force dependency on D3D11 in order that the D3D11.dll is loaded
static struct LoadD3D11 {
    LoadD3D11() {
        ID3D11Device *device = 0;
        D3D_FEATURE_LEVEL level = D3D_FEATURE_LEVEL_11_0;
        ID3D11DeviceContext *context = 0;
        HRESULT forceD3D11 = D3D11CreateDevice(
            0, D3D_DRIVER_TYPE_UNKNOWN, 0, 0, 0, 0,
            D3D11_SDK_VERSION, &device, &level, &context
        );
        if ( (forceD3D11 == S_OK) && (device != 0 ) )
            device->Release();
    }
} loadD3D11;
#endif //SUPPORT_D3D11

//-----------------------------------------------------------------------------

IDirect3D9 * WINAPI fake_Direct3DCreate9( UINT SDKVersion )
{
    if (Log::info())
        Log::print() << "Direct3DCreate9(" << SDKVersion << ")\n";

    IDirect3D9 *direct3D9 = 0;

    // the IDirect3D9Ex interface is only required for NV_DX_Interop
    // on Windows Vista upwards, where WDDM is used (this is configured
    // in the Settings class)
    if ( Settings::get().forceDirect3D9Ex ) {
        // attempt to load the Direct3DCreate9Ex function: will fail on XP 32
        // (but we shouldn't be in this branch on XP)
        PFNDirect3DCreate9Ex direct3DCreate9Ex =
            reinterpret_cast<PFNDirect3DCreate9Ex>(
                GetProcAddress( GetModuleHandle(L"d3d9"), "Direct3DCreate9Ex")
            );

        // if we failed to get the Direct3DCreate9Ex function, bail out now
        if ( direct3DCreate9Ex == 0 ) return 0;

        IDirect3D9Ex *direct3D9Ex = 0;

        // create a IDirect3D9Ex instance (instead of the requested
        // IDirect3D9 instance)
        HRESULT result = direct3DCreate9Ex(
            D3D_SDK_VERSION,
            &direct3D9Ex
        );

        if ( result != S_OK ) return 0;

        // downcast to IDirect3D9
        direct3D9 = dynamic_cast<IDirect3D9*>( direct3D9Ex );
    } else {
        // create the real IDirect3D9 instance
        direct3D9 = real_Direct3DCreate9( SDKVersion );
    }

    // create our proxy to wrap the real instance
    return new IDirect3D9Proxy( direct3D9 );
}

//-----------------------------------------------------------------------------

#if defined(SUPPORT_D3D11)
HRESULT WINAPI fake_D3D11CreateDevice(
    __in_opt IDXGIAdapter* pAdapter,
    D3D_DRIVER_TYPE DriverType,
    HMODULE Software,
    UINT Flags,
    __in_ecount_opt( FeatureLevels ) CONST D3D_FEATURE_LEVEL* pFeatureLevels,
    UINT FeatureLevels,
    UINT SDKVersion,
    __out_opt ID3D11Device** ppDevice,
    __out_opt D3D_FEATURE_LEVEL* pFeatureLevel,
    __out_opt ID3D11DeviceContext** ppImmediateContext
) {
    Log::print() << "D3D11CreateDevice\n";

    // create the real Direct3D 11 device
    HRESULT result = real_D3D11CreateDevice(
        pAdapter,
        DriverType,
        Software,
        Flags,
        pFeatureLevels,
        FeatureLevels,
        SDKVersion,
        ppDevice,
        pFeatureLevel,
        ppImmediateContext
    );

    Log::print() << "device=" << *ppDevice << " context=" << *ppImmediateContext << endl;

    // create our proxy device and substitute it for the real one
    if ( ppDevice != 0 ) {
        *ppDevice = new ID3D11DeviceProxy( *ppDevice );
    }

    // create our proxy device context and substitute it for the real one
    if ( ppImmediateContext != 0 ) {
        *ppImmediateContext = new ID3D11DeviceContextProxy(
            *ppImmediateContext
        );
    }

    return result;
}
#endif //SUPPORT_D3D11

//-----------------------------------------------------------------------------

#if defined(SUPPORT_D3D11)
HRESULT WINAPI fake_D3D11CreateDeviceAndSwapChain(
    _In_   IDXGIAdapter *pAdapter,
    _In_   D3D_DRIVER_TYPE DriverType,
    _In_   HMODULE Software,
    _In_   UINT Flags,
    _In_   const D3D_FEATURE_LEVEL *pFeatureLevels,
    _In_   UINT FeatureLevels,
    _In_   UINT SDKVersion,
    _In_   const DXGI_SWAP_CHAIN_DESC *pSwapChainDesc,
    _Out_  IDXGISwapChain **ppSwapChain,
    _Out_  ID3D11Device **ppDevice,
    _Out_  D3D_FEATURE_LEVEL *pFeatureLevel,
    _Out_  ID3D11DeviceContext **ppImmediateContext
) {
    Log::print() << "D3D11CreateDeviceAndSwapChain\n";

    HRESULT result = real_D3D11CreateDeviceAndSwapChain(
        pAdapter,
        DriverType,
        Software,
        Flags,
        pFeatureLevels,
        FeatureLevels,
        SDKVersion,
        pSwapChainDesc,
        ppSwapChain,
        ppDevice,
        pFeatureLevel,
        ppImmediateContext
    );

    Log::print() << "device=" << *ppDevice << " context=" << *ppImmediateContext << endl;

    // create our proxy swap chain and substitute it for the real one
    if ( ppSwapChain != 0 ) {
        *ppSwapChain = new DXGISwapChainProxy( *ppSwapChain );
    }

    return result;
}
#endif //SUPPORT_D3D11

//-----------------------------------------------------------------------------

#if defined(SUPPORT_D3D11)
HRESULT WINAPI fake_CreateDXGIFactory( REFIID riid, void **ppFactory )
{
    Log::print() << "CreateDXGIFactory\n";
    return real_CreateDXGIFactory( riid, ppFactory );
}
#endif //SUPPORT_D3D11

//-----------------------------------------------------------------------------

#if defined(SUPPORT_D3D11)
HRESULT WINAPI fake_CreateDXGIFactory1( REFIID riid, void **ppFactory )
{
    Log::print() << "CreateDXGIFactory1\n";

    // sanity check
    if ( riid != __uuidof(IDXGIFactory1) ) {
        Log::print() << "CreateDXGIFactory1 called with unexpected riid\n";
        return real_CreateDXGIFactory1( riid, (void**)(ppFactory) );
    }

    // create the real DXGIFactory
    IDXGIFactory1 *pFactory = 0;
    HRESULT result = real_CreateDXGIFactory1( riid, (void**)&pFactory );

    if ( result == S_OK ) {
        // create and substitute our proxy factory
        pFactory = reinterpret_cast<IDXGIFactory1*>(
            new DXGIFactory1Proxy( pFactory )
        );
    }

    // return a pointer to the factory
    *ppFactory = static_cast<void*>( pFactory );

    return result;
}
#endif //SUPPORT_D3D11

//-----------------------------------------------------------------------------

LONG WINAPI fake_ChangeDisplaySettingsEx(
    _In_  LPCTSTR lpszDeviceName,
    _In_  DEVMODE *lpDevMode,
    HWND hWnd,
    _In_  DWORD dwFlags,
    _In_  LPVOID lParam
) {
    // to prevent display mode being changed, we set the CDS_TEST flag
    if ( Settings::get().preventModeChange )
        dwFlags |= CDS_TEST;

    // call the real function
    return real_ChangeDisplaySettingsEx(
        lpszDeviceName,
        lpDevMode,
        hWnd,
        dwFlags,
        lParam
    );
}

//-----------------------------------------------------------------------------

HWND WINAPI fake_CreateWindowExA(
    __in DWORD dwExStyle,
    __in_opt LPCSTR lpClassName,
    __in_opt LPCSTR lpWindowName,
    __in DWORD dwStyle,
    __in int X,
    __in int Y,
    __in int nWidth,
    __in int nHeight,
    __in_opt HWND hWndParent,
    __in_opt HMENU hMenu,
    __in_opt HINSTANCE hInstance,
    __in_opt LPVOID lpParam
) {
    if (Log::info()) {
        Log::print() << "CreateWindowExA("
            << ((HIWORD(lpClassName)  != 0)?lpClassName:"?") << ','
            << ((HIWORD(lpWindowName) != 0)?lpWindowName:"?") << ','
            << X << ','
            << Y << ' '
            << nWidth << 'x'
            << nHeight << ")\n";
    }

    // call the real function
    return real_CreateWindowExA(
        dwExStyle,
        lpClassName,
        lpWindowName,
        dwStyle,
        X,
        Y,
        nWidth,
        nHeight,
        hWndParent,
        hMenu,
        hInstance,
        lpParam
    );
}

//-----------------------------------------------------------------------------

HWND WINAPI fake_CreateWindowExW(
    __in DWORD dwExStyle,
    __in_opt LPCWSTR lpClassName,
    __in_opt LPCWSTR lpWindowName,
    __in DWORD dwStyle,
    __in int X,
    __in int Y,
    __in int nWidth,
    __in int nHeight,
    __in_opt HWND hWndParent,
    __in_opt HMENU hMenu,
    __in_opt HINSTANCE hInstance,
    __in_opt LPVOID lpParam
) {
    USES_CONVERSION;

    if (Log::info()) {
        Log::print() << "CreateWindowExW("
            << ((HIWORD( lpClassName ) != 0) ? W2A( lpClassName ) : "?") << ','
            << ((HIWORD( lpWindowName ) != 0) ? W2A( lpWindowName ) : "?") << ','
            << X << ','
            << Y << ' '
            << nWidth << 'x'
            << nHeight << ")\n";
    }

    // call the real function
    return real_CreateWindowExW(
        dwExStyle,
        lpClassName,
        lpWindowName,
        dwStyle,
        X,
        Y,
        nWidth,
        nHeight,
        hWndParent,
        hMenu,
        hInstance,
        lpParam
    );
}

//-----------------------------------------------------------------------------

FARPROC WINAPI fake_GetProcAddress(
    HMODULE hModule,
    LPCSTR lpProcName
) {
    if ( HIWORD(lpProcName) == 0 ) {
        // likely to be an ordinal if high word is zero
    } else {
        // must be a string if high word is non-zero

        // get the module handle for d3d9.dll (static for performance)
        static HMODULE direct3D9 = GetModuleHandle( L"d3d9" );

        // substitute our fake Direct3DCreate9 function
        if ( hModule == direct3D9 ) {
            if ( strcmp( lpProcName, "Direct3DCreate9" ) == 0 )
                return reinterpret_cast<FARPROC>( fake_Direct3DCreate9 );
        }
    }

    return real_GetProcAddress( hModule, lpProcName );
}

//-----------------------------------------------------------------------------

void processAttach()
{
    Log::open( "intercept.log" );
    if (Log::info())
        Log::print( "DLL_PROCESS_ATTACH\n" );
    //MessageBox( 0, L"quadifier", L"debug me", MB_OK );

    // hook Direct3DCreate9
    if ( (real_Direct3DCreate9 == 0) ||
         !Mhook_SetHook( reinterpret_cast<PVOID*>(&real_Direct3DCreate9),
            fake_Direct3DCreate9 )
    )
        MessageBox( 0, L"Failed to hook Direct3DCreate9", L"Error", MB_OK );

    // Begin D3D11 hooking ----------------------------------------------------
    #if defined(SUPPORT_D3D11)
    // hook D3D11CreateDevice
    if ( (real_D3D11CreateDevice == 0) ||
            !Mhook_SetHook( reinterpret_cast<PVOID*>(&real_D3D11CreateDevice),
            fake_D3D11CreateDevice )
    )
        MessageBox( 0, L"Failed to hook D3D11CreateDevice", L"Error", MB_OK );

    // hook D3D11CreateDeviceAndSwapChain
    if ( (real_D3D11CreateDeviceAndSwapChain == 0) ||
            !Mhook_SetHook( reinterpret_cast<PVOID*>(&real_D3D11CreateDeviceAndSwapChain),
            fake_D3D11CreateDeviceAndSwapChain )
    )
        MessageBox( 0, L"Failed to hook D3D11CreateDeviceAndSwapChain", L"Error", MB_OK );

    // hook CreateDXGIFactory
    if ( (real_CreateDXGIFactory == 0) ||
            !Mhook_SetHook( reinterpret_cast<PVOID*>(&real_CreateDXGIFactory),
            fake_CreateDXGIFactory )
    )
        MessageBox( 0, L"Failed to hook CreateDXGIFactory", L"Error", MB_OK );

    // hook CreateDXGIFactory1
    if ( (real_CreateDXGIFactory1 == 0) ||
            !Mhook_SetHook( reinterpret_cast<PVOID*>(&real_CreateDXGIFactory1),
            fake_CreateDXGIFactory1 )
    )
        MessageBox( 0, L"Failed to hook CreateDXGIFactory1", L"Error", MB_OK );
    #endif //SUPPORT_D3D11
    // End D3D11 hooking ------------------------------------------------------

    // hook ChangeDisplaySettingsEx
    if ( (real_ChangeDisplaySettingsEx == 0) ||
         !Mhook_SetHook( reinterpret_cast<PVOID*>(&real_ChangeDisplaySettingsEx),
            fake_ChangeDisplaySettingsEx )
    )
        MessageBox( 0, L"Failed to hook ChangeDisplaySettingsEx", L"Error", MB_OK );

    // hook CreateWindowEx
    if ( (real_CreateWindowExA == 0) ||
         !Mhook_SetHook( reinterpret_cast<PVOID*>(&real_CreateWindowExA),
            fake_CreateWindowExA )
    )
        MessageBox( 0, L"Failed to hook CreateWindowExA", L"Error", MB_OK );

    // hook CreateWindowExW
    if ( (real_CreateWindowExW == 0) ||
         !Mhook_SetHook( reinterpret_cast<PVOID*>(&real_CreateWindowExW),
            fake_CreateWindowExW )
    )
        MessageBox( 0, L"Failed to hook CreateWindowExW", L"Error", MB_OK );

    // hook GetProcAddress so that we can return our fake Direct3DCreate9
    if ( (real_GetProcAddress == 0) ||
         !Mhook_SetHook( reinterpret_cast<PVOID*>(&real_GetProcAddress),
            fake_GetProcAddress )
    )
        MessageBox( 0, L"Failed to hook GetProcAddress", L"Error", MB_OK );
}

//-----------------------------------------------------------------------------

void processDetach() {
    if (Log::info())
        Log::print( "DLL_PROCESS_DETACH" );

    Mhook_Unhook( reinterpret_cast<PVOID*>(&real_GetProcAddress)  );
    Mhook_Unhook( reinterpret_cast<PVOID*>(&real_CreateWindowExW) );
    Mhook_Unhook( reinterpret_cast<PVOID*>(&real_CreateWindowExA) );
    Mhook_Unhook( reinterpret_cast<PVOID*>(&real_ChangeDisplaySettingsEx) );
    Mhook_Unhook( reinterpret_cast<PVOID*>(&real_Direct3DCreate9) );

    #if defined(SUPPORT_D3D11)
    Mhook_Unhook( reinterpret_cast<PVOID*>(&real_D3D11CreateDevice) );
    Mhook_Unhook( reinterpret_cast<PVOID*>(&real_D3D11CreateDeviceAndSwapChain) );
    #endif

    Mhook_Unhook( reinterpret_cast<PVOID*>(&real_CreateDXGIFactory) );
    Mhook_Unhook( reinterpret_cast<PVOID*>(&real_CreateDXGIFactory1) );
}

//-----------------------------------------------------------------------------

BOOL WINAPI DllMain(
    HINSTANCE instance,
    DWORD reason,
    LPVOID reserved
) {
    switch (reason) { 
    case DLL_PROCESS_ATTACH:
        DisableThreadLibraryCalls( instance ); 
        processAttach();
        break;

    case DLL_PROCESS_DETACH:
        processDetach();
        break;
    }

    // success
    return TRUE;
}

//-----------------------------------------------------------------------------
