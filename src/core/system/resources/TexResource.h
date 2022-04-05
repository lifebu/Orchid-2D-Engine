//////////////////////////////////////////////////////////////////
// Orchid 2D Framework
// Version: Alpha 0.1
// File: TexResource.h
// Purpose: - This is a Structure for the RessourceManager to
//            regulate Textureloading
//////////////////////////////////////////////////////////////////

// Preprocessor Directives:
#pragma once

// Includes
#include <d3d9.h>
#include <d3dx9.h>
#include <string>

/* The Vertex structure. */
struct TEXRESOURCE
{
	LPDIRECT3DTEXTURE9 texture;
	std::string texname;
	UINT referenceCounter;
};