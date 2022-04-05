//////////////////////////////////////////////////////////////////
// Orchid 2D Framework
// Version: Alpha 0.1
// File: GraphicDefinitions.h
// Purpose: - This is a Collection of Definitions for the Graphicpart
//////////////////////////////////////////////////////////////////

// Preprocessor Directives:
#pragma once

// Includes
#include <d3d9.h>
#include <d3dx9.h>

/* The Vertex structure. */
struct Vertex
{
	D3DXVECTOR3			position;				// Position of Vertex
	D3DCOLOR			diffuseColor;			// Color of Vertex
	D3DXVECTOR2			texture;				// Coordinates for the Textures
	static const DWORD FVF = D3DFVF_XYZ | D3DFVF_DIFFUSE | D3DFVF_TEX1;
};

/* Render Settings for Sprites. */
struct RenderSettings
{

};

/* Font Drawing Queue Struct.*/
struct FontDraw
{
	std::string string;
	int x;
	int y;
	D3DCOLOR color;
};