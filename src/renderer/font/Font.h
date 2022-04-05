//////////////////////////////////////////////////////////////////
// Orchid 2D Framework
// Version: Alpha 0.1
// File: Font.h
// Purpose: - This Class represents DirectX Fonts.
//////////////////////////////////////////////////////////////////

// Preprocessor Directives:
#pragma once

// Includes
#include <d3d9.h>
#include <d3dx9.h>
#include <cstdio>
#include <string>

class Font final
{
	// Functions:
	// Public functions:
public:
	Font();
	~Font();

	/* Initialize the Graphic. */
	void Initialize(const LPDIRECT3DDEVICE9 d3dDevice, const std::string& font, const DWORD& fontSize, 
				    const bool& italic, const bool& bold);
	/* Shutdown the Graphic. */
	void Shutdown();

	/* Write a text. */
	void Write(const std::string string, const int& x, const int& y, const D3DCOLOR& color);
	/* Is it initialized? */
	bool Initialized();


	// Private functions:
private:
	// Member:
	// Public Member:
public:

	// Private Member:
private:
	/* The actual Font. */
	LPD3DXFONT font;

	/* Is it initialized? */
	bool isInitialized;
};
