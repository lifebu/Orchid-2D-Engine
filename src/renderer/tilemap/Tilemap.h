//////////////////////////////////////////////////////////////////
// Orchid 2D Framework
// Version: Alpha 0.1
// File: Tilemap.h
// Purpose: - This Class is a Tilemap for a Tile-Based Game (orthogonal only)
//			- It contains some basic information about the tiles
//			- For Rendering, it also works like an Root Object of a Scene Graph
//			- Tiles are always rendered from the top-left to bottom-right (like in D3D).
//////////////////////////////////////////////////////////////////

// Preprocessor Directives:
#pragma once

// Includes
#include <string>
#include <vector>
#include <windows.h>
#include <tinyxml2.h>
#include "core/system/ErrorHandler.h"
#include "Tileset.h"
#include "Tilelayer.h"
#include "renderer/sprite/Static_Sprite.h"
#include "renderer/sprite/Animated_Sprites.h"

// Libraries
#pragma comment(lib, "tinyxml2.lib")



/* Tilemap Class. */
class Tilemap
{
	// Functions:
	// Public functions:
public:
	Tilemap();
	~Tilemap();

	/* Initialize the Tilemap. */
	void Initialize(const std::string tmxFile, const LPDIRECT3DDEVICE9 d3dDevice);
	/* Shutdown the Tilemap. */
	void Shutdown();

	/* Is the Map Initialized? */
	bool IsInitialized() const;
	const std::vector<Tileset>& GetTilesets() const;
	const std::vector<Static_Sprite>& GetBackgroundImageLayers() const;
	const std::vector<Tilelayer>& GetBackgroundTileLayers() const;
	const std::vector<int>& GetBackgroundCollisionLayer() const;
	const std::vector<Animated_Sprite*>& GetObjectLayer() const;
	const std::vector<Tilelayer>& GetForegroundTileLayers() const;
	const std::vector<Static_Sprite>& GetEffectImageLayers() const;
	const UINT& GetTileWidth() const;
	const UINT& GetTileHeight() const;
	const UINT& GetMapWidth() const;
	const UINT& GetMapHeight() const;

	// Manipulation Functions:
	void AddObject(Animated_Sprite* newObject);

	// Private functions:
private:

	// Member:
	// Public Member:
public:

	// Private Member:
private:
	/* The Width of one Tile. */
	UINT tileWidth;
	/* The Height of one Tile. */
	UINT tileHeight;
	/* Width of the Map (Amount of Tiles). */
	UINT mapWidth;
	/* Height of the Map (Amount of Tiles). */
	UINT mapHeight;
	/* Is the Map Initialized? */
	bool isInitialized;
	/* All the Tilesets. */
	std::vector<Tileset> tileSets;

	/* All the Layers in the Order they are rendered: */
	std::vector<Static_Sprite> backroundImageLayers;
	std::vector<Tilelayer> backgroundTilelayers;
	std::vector<int> backgroundCollisionLayer;
	std::vector<Animated_Sprite*> objectLayer;
	std::vector<Tilelayer> foregroundTileLayers;
	std::vector<Static_Sprite> effectImageLayers;
};