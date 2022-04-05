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

// Includes
#include "Tileset.h"

// Functions:
// Public functions:
Tileset::Tileset()
	:
	firstGID(1),
	initialized(false),
	tilesPerHeight(0),
	tilesPerWidth(0),
	lastGID(2)
{

}
Tileset::~Tileset()
{

}

void Tileset::Initialize(const std::string textureFile, const UINT tileHeight, const UINT tileWidth,
	const UINT firstGID)
{
	HRESULT result;
	LPDIRECT3DTEXTURE9 tempTex;
	ErrorHandler* errorHandler = ErrorHandler::GetInstance();

	// 1st: Set Member:
	this->textureFile = textureFile;
	this->firstGID = firstGID;


	// 2nd: Load the Texture
	tempTex = ResourceManager::GetInstance()->LoadTexture(textureFile);
	if (tempTex == nullptr)
	{
		return;
	}

	// 3rd: Create the Texture Description
	result = tempTex->GetLevelDesc(0, &textureDesc);
	if (FAILED(result))
	{
		char buffer[256];
		sprintf(buffer, "Tileset.Initialize(): Failed to Get the Texture Description: %s, %s", DXGetErrorStringA(result), DXGetErrorDescriptionA(result));
		errorHandler->CreateFatalError(buffer);
		return;
	}

	// 4th: Set the other Member:
	tilesPerWidth = textureDesc.Width / tileWidth;
	tilesPerHeight = textureDesc.Height / tileHeight;
	lastGID = firstGID + ((tilesPerWidth * tilesPerHeight) - 1);

	initialized = true;


}
void Tileset::Shutdown()
{
	// Unload the Texture!
	ResourceManager::GetInstance()->UnloadTexture(textureFile);
	textureFile = "";
	initialized = false;
}

void Tileset::GetTextureCoords(UINT GID, float& minTexCoordX, float& maxTexCoordX, float& minTexCoordY, float& maxTexCoordY) const
{
	if (GID > firstGID + ((tilesPerWidth * tilesPerHeight) - 1)
		|| GID < firstGID)
	{
		// This ID is higher than the ID of this Tileset
		return;
	}

	if (!initialized)
	{
		// Not Initialized!
		return;
	}

	// 1st: Calculate the Coordinates
	/* -1 because GID is not zero based, but the Coordinate needs to be*/
	float x;
	float y;
	x = (float)((GID - firstGID) % tilesPerWidth);
	y = (float)((UINT)((GID - firstGID) / tilesPerWidth));


	minTexCoordX = x * (1.0f / (float)tilesPerWidth);
	maxTexCoordX = minTexCoordX + (1.0f / (float)tilesPerWidth);
	minTexCoordY = y * (1.0f / (float)tilesPerHeight);
	maxTexCoordY = minTexCoordY + (1.0f / (float)tilesPerHeight);	
}
const LPDIRECT3DTEXTURE9 Tileset::GetTexture() const
{
	return ResourceManager::GetInstance()->GetTexture(textureFile);
}
bool Tileset::IsInitialized() const
{
	return initialized;
}

const UINT& Tileset::GetFirstGID() const
{
	return firstGID;
}
const UINT& Tileset::GetLastGID() const
{
	return lastGID;
}

bool Tileset::IsGIDInTilesetRange(UINT GID) const
{
	if (GID >= firstGID && GID <= lastGID)
	{
		return true;
	}

	return false;
}