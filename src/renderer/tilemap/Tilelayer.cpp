//////////////////////////////////////////////////////////////////
// Orchid 2D Framework
// Version: Alpha 0.1
// File: Tilelayer.cpp
// Purpose: - The Tilelayer wraps up a lot of tiles in one layer
//			- One Layer has one depth
//			- It has a coordinate system for the tiles
//////////////////////////////////////////////////////////////////

// Includes
#include "Tilelayer.h"

// Functions:
// Public functions:
Tilelayer::Tilelayer()
	:
	height(1),
	width(1),
	isInitialized(false)
{
}
Tilelayer::~Tilelayer()
{
}

void Tilelayer::Initialize(const UINT& width, const UINT& height, const std::vector<Tile>& tiles, std::string name)
{
	if (tiles.size() != (width * height))
	{
		// Need to have the right size!
		return;
	}

	// 1st: Set Member:
	this->width = width;
	this->height = height;
	this->tiles = tiles;
	this->name = name;
	isInitialized = true;

	// 2nd: Set the Positions of the tiles
	for (auto& entry : tiles)
	{
		
	}
}
void Tilelayer::Shutdown()
{
	/*for (auto& entry : tiles)
	{
		entry.Shutdown();
	}*/

	isInitialized = false;
}

bool Tilelayer::IsInitialized() const
{
	return isInitialized;
}
const Tile& Tilelayer::getTile(int x, int y) const
{
	return tiles.at(y * (width + x));
}
const std::vector<Tile>& Tilelayer::getTiles() const
{
	return tiles;
}
const std::string& Tilelayer::getName() const
{
	return name;
}