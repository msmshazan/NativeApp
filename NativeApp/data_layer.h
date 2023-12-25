#pragma once

#include "common.h"

// Windows 
#define NOMINMAX 
#define WIN32_LEAN_AND_MEAN
#include <windows.h>

// SQL
#include <sql.h>
#include <sqlext.h>
#include <oledb.h>

// SDL2
#include <SDL2/SDL.h>
#include <SDL2/SDL_main.h>
#include <SDL2/SDL_syswm.h>


namespace DB {

	// Data
	API void* Data;
	API bool IsConnectedToDatabase;

	// Functions
	API void ConnectToDatabase();

}