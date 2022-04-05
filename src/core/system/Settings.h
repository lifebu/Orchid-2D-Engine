//////////////////////////////////////////////////////////////////
// Orchid Engine
// Version 0.1
// File: D3DSettings.h
// Purpose: TypeDef: Settings-Struct.
//////////////////////////////////////////////////////////////////

// Preprocessor Directives
#pragma once

// Includes:
#include <windows.h>
#include <d3d9types.h>

/* Settings for the Engine. */
struct SETTINGS
{
	// Foreground Settings:
	// Graphic Settings:
	bool hardwareRendering;
	bool windowed;
	UINT resolutionWidth;
	UINT resolutionHeight;
	UINT bufferCount;
	bool vSync;
	D3DMULTISAMPLE_TYPE multisampleType;
	DWORD               multisampleQuality;

	// Background Settings:
	// Graphics Settings:
	D3DFORMAT           backbufferFormat;
	D3DSWAPEFFECT       swapEffect;
	HWND                deviceWindow;
	bool                enableAutoDepthStencil;
	D3DFORMAT           autoDepthStencilFormat;
	DWORD               flags;
	UINT                fullscreen_RefreshRateInHz;
};