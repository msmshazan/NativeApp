#pragma once

/*
	Headers
*/

#include "common.h"
// Windows 
#define NOMINMAX 
#define WIN32_LEAN_AND_MEAN
#include <windows.h>

// SQL
#include <sql.h>
#include <sqlext.h>
// Direct3d
#include <dxgi.h>
#include <d3d11.h>
// SDL2
#include <SDL2/SDL.h>
#include <SDL2/SDL_main.h>
#include <SDL2/SDL_syswm.h>
// ImGui Platform
#include "imgui_impl_sdl2.h"
#include "imgui_impl_dx11.h"
// Skia
#include "include/core/SkCanvas.h"
#include "dx11_helpers.h"