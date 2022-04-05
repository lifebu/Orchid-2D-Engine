//////////////////////////////////////////////////////////////////
// Orchid 2D Framework
// Version: Alpha 0.1
// File: Tileset.h
// Purpose: - This Class is a Tileset for Orthogonal Tilemap Games
//			- It stores all different Textures (Every tile is it's own texture).
//			- It uses a Tileset ID to refere to a certain Texture of the Tileset
//			- It is only important to load new Tiles 
//				- existing ones already got a pointer to the texture
//////////////////////////////////////////////////////////////////

// Preprocessor Directives:
#pragma once

// Includes
#include <string>
#include <vector>
#include <d3dx9.h>
#include "core/system/ErrorHandler.h"
#include "core/system/resources/ResourceManager.h"
#include "core/system/resources/TexResource.h"

/* Tileset Class. */
class Tileset
{
	// Functions:
	// Public functions:
public:
	Tileset();
	~Tileset();

	/* Initialize the Tileset. 
	 * ID IS NOT ZERO-BASED!!! */
	void Initialize(const std::string textureFile, const UINT tileHeight, const UINT tileWidth,
		const UINT firstGID);
	/* Shutdown the Tileset. */
	void Shutdown();

	void GetTextureCoords(UINT GID, float& minTexCoordX, float& maxTexCoordX, float& minTexCoordY, float& maxTexCoordY) const;
	const LPDIRECT3DTEXTURE9 GetTexture() const;
	bool IsInitialized() const;

	const UINT& GetFirstGID() const;
	const UINT& GetLastGID() const;
	bool IsGIDInTilesetRange(UINT GID) const;


	// Private functions:
private:

	// Member:
	// Public Member:
public:

	// Private Member:
private:
	/* TextureDescription. */
	D3DSURFACE_DESC textureDesc;
	/* The firstGID of the Tileset (.tmx file is not zero-based!)
	 * Like an offset to the ID if you use multiple tilesets.*/
	UINT firstGID;
	/* The Last GID of the Tileset. */
	UINT lastGID;
	
	UINT tilesPerWidth;
	UINT tilesPerHeight;
	/* Is it Initialized?*/
	bool initialized;
	
	/* Name of the Texture File. */
	std::string textureFile;

	ErrorHandler* errorHandler;

};