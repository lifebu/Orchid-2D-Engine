//////////////////////////////////////////////////////////////////
// Orchid 2D Framework
// Version: Alpha 0.1
// File: ResourceManager.h
// Purpose: ResourceManager Class for handeling allocation etc.
// Tasks:
//		- TODO:
//			- Care for Memory Alignment
//			- Multithread Access and so on.
//			- Extra Bytes after and before Data for Memory Corruption detection.
//			- Track WHERE Memory was allocated
//			- Track all Memory allocation
//			- Functions for Allocation
//			- Statistics
//			- bla 
//////////////////////////////////////////////////////////////////

// Preprocessor Directives
#pragma once

// Includes
#include <vector>
#include <map>
#include <string>
#include <d3d9.h>
#include <DxErr.h>
#include <windows.h>
#include "core/system/ErrorHandler.h"
#include "AllocatedMemory.h"
#include "TexResource.h"

/* Resource and File Manager. */
class ResourceManager final
{
	// Functions:
	// Public Functions:
public:
	ResourceManager();
	~ResourceManager();

	static ResourceManager* GetInstance();

	void Initialize(LPDIRECT3DDEVICE9* d3dDevice);
	void Shutdown();

	void* Allocate(const int size, const std::string File, const int Line);
	void DeAllocate(const void*& Memory);

	std::vector<TEXRESOURCE> getTex()
	{
		return textureResources;
	}

	void OutputUsedMemory();

	// Texture Functions
	/* The Resourcemanager creats a new texture, if it doesn't exist.
	 * If it exists, the reference counter is incremented and a pointer
	 *  is returned (no double textures).*/
	LPDIRECT3DTEXTURE9 LoadTexture(std::string textureName);
	/* With this function an existing texture is added to the list
	 * of textures */
	void IntegrateExistingTexture(std::string textureName, LPDIRECT3DTEXTURE9 texture);
	/* A Sprite tells the Ressourcemanager that it wants to shutdown
	 * decreases the reference Counter of the Texture and if its 0,
	 * then it will release it*/
	void UnloadTexture(std::string textureName);
	/* Releases all Textures to prepare for a device reset.*/
	void ReleaseTextures();
	/* Reloads all the Textures after a device reset. */
	void ReloadTextures();

	// Getters
	/* Get the texture without incrementing the Counter. */
	LPDIRECT3DTEXTURE9 GetTexture(std::string textureName);

	// Member:
	// Private Member:
private:
	// Own Instance
	static ResourceManager *instance;
	LPDIRECT3DDEVICE9* d3dDevice;

	std::vector<AllocatedMemory> allocatedMemory;
	int usedMemory;

	// For Textures
	std::vector<TEXRESOURCE> textureResources;

	ErrorHandler* errorHandler;
};

