//////////////////////////////////////////////////////////////////
// Orchid 2D Framework
// Version: Alpha 0.1
// File: Tilelayer.h
// Purpose: - The Tilelayer wraps up a lot of tiles in one layer
//			- One Layer has one depth
//			- It has a coordinate system for the tiles
//////////////////////////////////////////////////////////////////

// Preprocessor Directives:
#pragma once

// Includes
#include <string>
#include <vector>
#include <d3dx9.h>
#include "core/system/ErrorHandler.h"
#include "core/system/resources/ResourceManager.h"
#include "Tile.h"

/* Tileset Class. */
class Tilelayer
{
	// Functions:
	// Public functions:
public:
	Tilelayer();
	~Tilelayer();

	/* Initialize the Tilelayer. */
	void Initialize(const UINT& width, const UINT& height, const std::vector<Tile>& tiles, std::string name);
	/* Shutdown the Tilelayer. */
	void Shutdown();

	bool IsInitialized() const;

	const Tile& getTile(int x, int y) const;
	const std::vector<Tile>& getTiles() const;
	const std::string& getName() const;


	// Private functions:
private:

	// Member:
	// Public Member:
public:

	// Private Member:
private:
	/* The Tilelayer is initialized. */
	bool isInitialized;
	/* The vector for the tiles. 
	 * it's used 2D: y * (width + x).*/
	std::vector<Tile> tiles;
	/* width of the layer. */
	UINT width;
	/* height of the layer. */
	UINT height;
	/* Name of the Layer. */
	std::string name;

};