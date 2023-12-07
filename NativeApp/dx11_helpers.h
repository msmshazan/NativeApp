#pragma once

#include "common.h"
// Windows 
#define NOMINMAX 
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
// Direct3d
#include <dxgi.h>
#include <d3d11.h>

// Data
API ID3D11Device* g_pd3dDevice;
API ID3D11DeviceContext* g_pd3dDeviceContext;
API IDXGISwapChain* g_pSwapChain;
API ID3D11RenderTargetView* g_mainRenderTargetView;

// Functions
API void CreateRenderTarget();
API void CleanupRenderTarget();
API bool CreateDeviceD3D(HWND hWnd);
API void CleanupDeviceD3D();