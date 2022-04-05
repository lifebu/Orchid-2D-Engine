//////////////////////////////////////////////////////////////////
// Orchid 2D Framework
// Version: Alpha 0.1
// File: Tilemap.cpp
// Purpose: - This Class is a Tilemap for a Tile-Based Game (orthogonal only)
//			- It contains some basic information about the tiles
//			- For Rendering, it also works like an Root Object of a Scene Graph
//			- Tiles are always rendered from the top-left to bottom-right (like in D3D).
//////////////////////////////////////////////////////////////////

// Includes
#include "Tilemap.h"


// Functions:
// Public functions:
Tilemap::Tilemap()
	:
	mapHeight(0),
	mapWidth(0),
	tileHeight(0),
	tileWidth(0),
	isInitialized(false)
{

}
Tilemap::~Tilemap()
{

}

void Tilemap::Initialize(const std::string tmxFile, const LPDIRECT3DDEVICE9 d3dDevice)
{
	// Data:
	/* The Whole XML-File. */
	tinyxml2::XMLDocument file;
	/* The root of the XML-File. */
	tinyxml2::XMLNode* rootNode;
	/* Result of the XML-Parser Functions. */
	int result;
	/* Temp Values for Query. */
	int tempVal;
	/* Temp String for Conversion. */
	std::string tempString;
	ErrorHandler* errorHandler = ErrorHandler::GetInstance();

	// 1st: Load the .tmx file
	result = file.LoadFile(tmxFile.c_str());
	if (result != 0)
	{
		char buffer[256];
		sprintf(buffer, "Tilemap.Initialize(): Failed to Load the TMX-File: %d", result);
		errorHandler->CreateFatalError(buffer);
		return;
	}

	// 2nd: Go through the .tmx file
	rootNode = file.FirstChild();
	
	// 2.2. Map:
	// 2.2.1. Check General Infos:
	tinyxml2::XMLNode* MapNode = rootNode->NextSibling();
	tinyxml2::XMLElement* MapElement = MapNode->ToElement();
	tempString = MapElement->Attribute("orientation");
	if (tempString != "orthogonal")
	{
		char buffer[256];
		sprintf(buffer, "Tilemap.Initialize(): TMX-File not orthogonal.");
		errorHandler->CreateFatalError(buffer);
		return;
	}
	tempString = MapElement->Attribute("renderorder");
	if (tempString != "right-down")
	{
		char buffer[256];
		sprintf(buffer, "Tilemap.Initialize(): TMX-File has not right-down renderorder!.");
		errorHandler->CreateFatalError(buffer);
		return;
	}

	// 2.2.2. Set Members:
	// Width:
	result = MapElement->QueryIntAttribute("width", &tempVal);
	if (result != 0)
	{
		char buffer[256];
		sprintf(buffer, "Tilemap.Initialize(): Failed to Get the Width of the Map from File: %d", result);
		errorHandler->CreateFatalError(buffer);
		return;
	}
	mapWidth = (UINT)tempVal;

	// Height:
	result = MapElement->QueryIntAttribute("height", &tempVal);
	if (result != 0)
	{
		char buffer[256];
		sprintf(buffer, "Tilemap.Initialize(): Failed to Get the Height of the Map from File: %d", result);
		errorHandler->CreateFatalError(buffer);
		return;
	}
	mapHeight = (UINT)tempVal;

	// tileWidth:
	result = MapElement->QueryIntAttribute("tilewidth", &tempVal);
	if (result != 0)
	{
		char buffer[256];
		sprintf(buffer, "Tilemap.Initialize(): Failed to Get the TileWidth of the Map from File: %d", result);
		errorHandler->CreateFatalError(buffer);
		return;
	}
	tileWidth = (UINT)tempVal;

	// tileWidth:
	result = MapElement->QueryIntAttribute("tileheight", &tempVal);
	if (result != 0)
	{
		char buffer[256];
		sprintf(buffer, "Tilemap.Initialize(): Failed to Get the TileHeight of the Map from File: %d", result);
		errorHandler->CreateFatalError(buffer);
		return;
	}
	tileHeight = (UINT)tempVal;

	// The Elements of the Map
	for (tinyxml2::XMLElement* currentElement = MapElement->FirstChildElement("tileset");
		currentElement != nullptr;
		currentElement = currentElement->NextSiblingElement())
	{
		tempString = currentElement->Value();
		if (tempString == "tileset")
		{
			// 2.3. A Tileset
			// Data:
			UINT firstGID;
			std::string fileName;
			tinyxml2::XMLElement* imageElement;
			Tileset newTileSet;
			
			// 2.3.1. FirstGID
			result = currentElement->QueryIntAttribute("firstgid", &tempVal);
			if (result != 0)
			{
				char buffer[256];
				sprintf(buffer, "Tilemap.Initialize(): Failed to Get the firstGID of the Tileset from File: %d", result);
				errorHandler->CreateFatalError(buffer);
				return;
			}
			firstGID = (UINT)tempVal;

			// 2.3.2. Image
			imageElement = currentElement->FirstChildElement("image");
			const char* tempchar = imageElement->Attribute("source");
			if (tempchar == nullptr)
			{
				char buffer[256];
				sprintf(buffer, "Tilemap.Initialize(): Failed to Get the Image Source of the Tileset: %d", result);
				errorHandler->CreateFatalError(buffer);
				return;
			}
			fileName = tempchar;

			newTileSet.Initialize(fileName, tileHeight, tileWidth, firstGID);
			tileSets.push_back(newTileSet);
			
			int a = 10;
		}
		else if (tempString == "layer")
		{
			// 2.4. A Layer
			// Data
			Tilelayer newLayer;
			UINT width;
			UINT height;
			tinyxml2::XMLElement* tileDataElement;
			std::vector<Tile> tiles;

			// 2.4.1. Width
			result = currentElement->QueryIntAttribute("width", &tempVal);
			if (result != 0)
			{
				char buffer[256];
				sprintf(buffer, "Tilemap.Initialize(): Failed to Get the Width of the Tilelayer from File: %d", result);
				errorHandler->CreateFatalError(buffer);
				return;
			}
			width = (UINT)tempVal;

			result = currentElement->QueryIntAttribute("height", &tempVal);
			if (result != 0)
			{
				char buffer[256];
				sprintf(buffer, "Tilemap.Initialize(): Failed to Get the Width of the Tilelayer from File: %d", result);
				errorHandler->CreateFatalError(buffer);
				return;
			}
			height = (UINT)tempVal;

			// 2.4.2. Tile Data
			tileDataElement = currentElement->FirstChildElement("data");
			UINT tileElementNumber = 0;
			for (tinyxml2::XMLElement* tileElement = tileDataElement->FirstChildElement("tile");
				tileElement != nullptr;
				tileElement = tileElement->NextSiblingElement())
			{
				// It's iterating through all tiles
				// Data:
				Tile tile;
				UINT tileGID;
				Tileset neededTileset;

				result = tileElement->QueryIntAttribute("gid", &tempVal);
				if (result != 0)
				{
					char buffer[256];
					sprintf(buffer, "Tilemap.Initialize(): Failed to Get the Width of the Tilelayer from File: %d", result);
					errorHandler->CreateFatalError(buffer);
					return;
				}
				tileGID = (UINT)tempVal;

				// 2.4.2.1. Get the Tileset which is the right for this GID.
				for (auto& entry : tileSets)
				{
					if (tileGID >= entry.GetFirstGID() &&
						tileGID <= entry.GetLastGID())
					{
						neededTileset = entry;
					}
				}

				// 2.4.2.2. Calculate the Position for this Tile.

				/* MiddlePos is seen from the top left point of the map. */
				D3DXVECTOR3 MiddleMapPos;
				MiddleMapPos.x = ((float)(mapWidth * tileWidth)) / 2.0f;
				MiddleMapPos.y = ((float)(mapHeight * tileHeight)) / 2.0f;
				MiddleMapPos.z = 0;

				/* Graphical offset for the Tile (tilesposition are defined via the middle of the tile!). */
				D3DXVECTOR3 MiddleTilePos;
				MiddleTilePos.x = ((float)tileWidth) / 2.0f;
				MiddleTilePos.y = ((float)tileHeight) / 2.0f;
				MiddleTilePos.z = 0;

				UINT x = tileElementNumber % mapWidth;
				UINT y = ( mapHeight - (UINT)((float)tileElementNumber / (float)mapWidth));

				D3DXVECTOR3 TilePos;
				TilePos.x = (float)(x * tileWidth);
				TilePos.y = (float)(y * tileHeight);
				TilePos.z = 20.0f;

				// Correction for the Middle of the Tile:
				TilePos += MiddleTilePos;

				// Correction for the Middle of the Map
				TilePos -= MiddleMapPos;

				tile.Initialize(d3dDevice, TilePos, tileGID, neededTileset,
				tileWidth, tileHeight);

				tiles.push_back(tile);

				tileElementNumber++;
			}

			std::string layername = currentElement->Attribute("name");
			newLayer.Initialize(width, height, tiles, layername);

			// Check which layer this is:

			if (layername.find("Background") != std::string::npos)
			{
				// The Tilelayername Contains "Background", so this is a background tile layer
				backgroundTilelayers.push_back(newLayer);
			}
			else if (layername.find("Foreground") != std::string::npos)
			{
				// The Tilelayername contains "Foreground", so this is a foreground tile layer
				foregroundTileLayers.push_back(newLayer);
			}

		}
		else if (tempString == "imagelayer")
		{
			// 2.5. A Imagelayer
			tinyxml2::XMLElement* imageElement = currentElement->FirstChildElement("image");
			if (imageElement == nullptr)
			{
				// This Imagelayer has now image assigned to it, so skip this layer.
				continue;
			}
			std::string texturefile = imageElement->Attribute("source");

			Static_Sprite newSprite;
			newSprite.Initialize(d3dDevice, { 64.0f, -484.0f, 20.0f }, texturefile);

			// Check which layer this is:
			std::string layername = currentElement->Attribute("name");
			if (layername.find("Background") != std::string::npos)
			{
				// The Imagelayername Contains "Background", so this is a background image layer
				backroundImageLayers.push_back(newSprite);
			}
			else if (layername.find("Effect") != std::string::npos)
			{
				// The Imagelayername contains "Effect", so this is a Effect image layer
				effectImageLayers.push_back(newSprite);
			}
		}
	}

	isInitialized = true;
	
}
void Tilemap::Shutdown()
{
	for (auto& entry : tileSets)
	{
		entry.Shutdown();
	}

	for (auto& entry : backroundImageLayers)
	{
		entry.Shutdown();
	}

	for (auto& entry : backgroundTilelayers)
	{
		entry.Shutdown();
	}

	for (auto& entry : backgroundCollisionLayer)
	{
		entry = 0;
	}

	objectLayer.clear();

	for (auto& entry : foregroundTileLayers)
	{
		entry.Shutdown();
	}

	for (auto& entry : effectImageLayers)
	{
		entry.Shutdown();
	}

	isInitialized = false;
}

bool Tilemap::IsInitialized() const
{
	return isInitialized;
}
const std::vector<Tileset>& Tilemap::GetTilesets() const
{
	return tileSets;
} 
const std::vector<Static_Sprite>& Tilemap::GetBackgroundImageLayers() const
{
	return backroundImageLayers;
}
const std::vector<Tilelayer>& Tilemap::GetBackgroundTileLayers() const
{
	return backgroundTilelayers;
}
const std::vector<int>& Tilemap::GetBackgroundCollisionLayer() const
{
	return backgroundCollisionLayer;
}
const std::vector<Animated_Sprite*>& Tilemap::GetObjectLayer() const
{
	return objectLayer;
}
const std::vector<Tilelayer>& Tilemap::GetForegroundTileLayers() const
{
	return foregroundTileLayers;
}
const std::vector<Static_Sprite>& Tilemap::GetEffectImageLayers() const
{
	return effectImageLayers;
}
const UINT& Tilemap::GetTileWidth() const
{
	return tileWidth;
}
const UINT& Tilemap::GetTileHeight() const
{
	return tileHeight;
}
const UINT& Tilemap::GetMapWidth() const
{
	return mapWidth;
}
const UINT& Tilemap::GetMapHeight() const
{
	return mapHeight;
}

// Manipulation Functions:
void Tilemap::AddObject(Animated_Sprite* newObject)
{
	if (newObject != nullptr)
	{
		if (newObject->IsInitialized())
		{
			objectLayer.push_back(newObject);
		}
	}
}