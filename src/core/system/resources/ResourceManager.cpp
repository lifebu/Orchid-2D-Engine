//////////////////////////////////////////////////////////////////
// Orchid 2D Framework
// Version: Alpha 0.1
// File: ResourceManager.cpp
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

// Includes
#include "ResourceManager.h"

// Functions:
// Public Functions:
ResourceManager::ResourceManager()
	:
	usedMemory(0),
	errorHandler(ErrorHandler::GetInstance())
{
}
ResourceManager::~ResourceManager()
{
}

ResourceManager* ResourceManager::GetInstance()
{
	if (instance == nullptr)
	{
		instance = new ResourceManager();
	}
	return instance;
}

void ResourceManager::Initialize(LPDIRECT3DDEVICE9* d3dDevice)
{
	errorHandler->WriteLog("Initialiazing ResourceManager ...");
	this->d3dDevice = d3dDevice;
	errorHandler->WriteLog("ResourceManager Initialized.");
	return;
}
void ResourceManager::Shutdown()
{
	bool errors = false;
	// 1st: DeAllocate all Memory that has been allocated but not deallocated in the program.
	// And create a Message!
	errorHandler->WriteLog("Shutdown ResourceManager ...");

	if (ResourceManager::GetInstance()->allocatedMemory.size() > 0)
	{
		// 2nd: There is still not deallocated memory!
		errors = true;
		auto iterator = ResourceManager::GetInstance()->allocatedMemory.begin();
		while (iterator != ResourceManager::GetInstance()->allocatedMemory.end())
		{
			errorHandler->CreateMinorError("ResourceManager: Memory was not deallocated.");
			char buffer[256];
			sprintf(buffer, "You forgot to deallocate memory, which was allocated in Line %d of the File \n %s\n", iterator->line, iterator->file.c_str());
			errorHandler->CreateMinorError(buffer);
			free(iterator->memory);
			iterator = ResourceManager::GetInstance()->allocatedMemory.erase(iterator);

		}
	}

	// 3rd: There are still loaded Textures!
	if (textureResources.size() > 0)
	{
		errors = true;
		errorHandler->CreateMinorError("ResourceManager: Textures were not unloaded.");
		char buffer[256];
		auto iterator = ResourceManager::GetInstance()->textureResources.begin();
		while (iterator != ResourceManager::GetInstance()->textureResources.end())
		{
			// 3.1.: Write the Message:
			sprintf(buffer, "You forgot to unload textures: Filename: %s\n", iterator->texname.c_str());
			errorHandler->CreateMinorError(buffer);

			// 3.2.: Unload Texture
			iterator->referenceCounter = 0;
			iterator->texture->Release();
			iterator = ResourceManager::GetInstance()->textureResources.erase(iterator);
		}
	}

	if (!errors)
	{
		errorHandler->WriteLog("ResourceManager Shutdown successful.");
	}

	// 3rd: Delete the Instance.
	if (instance != nullptr)
	{
		delete instance;
		instance = nullptr;
	}

	return;
}

void* ResourceManager::Allocate(const int size, const std::string File, const int Line)
{
	if (size > 0)	// You can't allocate 0 Bytes of memory!
	{

		// 1st: Allocate new Memory
		void* newMemory = malloc(size);

		// 2nd: Create a new AllocatedMemory struct and fill the Data
		AllocatedMemory newAllocation;
		newAllocation.memory = newMemory;
		newAllocation.file = File;
		newAllocation.line = Line;
		newAllocation.size = size;

		// 3rd: Add the new Struct to the Memorylist
		allocatedMemory.push_back(newAllocation);

		// 4th: Increase the UsedMemory Counter
		usedMemory += size;

		// 5th: return the new Memory
		return newAllocation.memory;
	}

	return nullptr;
}
void ResourceManager::DeAllocate(const void*& Memory)
{
	int test = sizeof(*&Memory);

	auto iterator = allocatedMemory.begin();
	while (iterator != allocatedMemory.end())
	{
		if (Memory == (iterator->memory))	// 1st: Is this a Memory which was allocated via the ResourceManager?
		{
			// 2nd: Yes, so free it
			free(iterator->memory);
			iterator->memory = NULL;
			usedMemory -= iterator->size;
			iterator = allocatedMemory.erase(iterator);
			Memory = NULL;
			return;
		}
	}
}

void ResourceManager::OutputUsedMemory()
{
	printf("Used Memory: %d \n", usedMemory);
}

// Texture Functions:
LPDIRECT3DTEXTURE9 ResourceManager::LoadTexture(std::string textureName)
{
	HRESULT result;
	

	if (textureResources.size() > 0)
	{
		// 1st: Check if this texture was already loaded
		for (auto& entry : textureResources)
		{
			if (entry.texname == textureName)
			{
				// Already loaded
				entry.referenceCounter++;
				return entry.texture;
			}
			
		}

		// It has not returned yet so this texture does not exist, so load it!
		TEXRESOURCE temp;
		temp.texture = nullptr;
		temp.texname = textureName;
		temp.referenceCounter = 1;

		// 1st: Create the Texture
		result = D3DXCreateTextureFromFileExA(*d3dDevice,					// The Direct3DDevice.
											  temp.texname.c_str(),		// Name of the File.
											  D3DX_DEFAULT,					// Width of the Texture (Here: Use the Width of the Imagefile).
											  D3DX_DEFAULT,					// Height of the Texture (Here: Use the Width of the Imagefile).
											  1,							// Amount of MIP-Map-Layer (Here: Create as much as possible).
											  0,							// Usage of the Texture (Here: Static Texture).
											  D3DFMT_UNKNOWN,				// Dataformat of the Texture (Here: Use the Format of the Imagefile).
											  D3DPOOL_DEFAULT,				// How the Texture is handled (Default: You need to worry about
																			// Lost Devices, but you can use special functions). 
											  D3DX_FILTER_NONE,				// Filter for Creating the Texture (Here: None).
											  D3DX_DEFAULT,					// Filter for Creating the Mip-Maps (Here: None).
											  0,							// ColorKey (The defined color is transparent!)
											  NULL,							// Always NULL
											  NULL,							// Only interesting for Pallettextures (8-Bit), otherwise NULL.
											  &temp.texture);				// Address to the Texture, that need to be created.
		if (FAILED(result))
		{
			char buffer[256];
			sprintf(buffer, "ResourceManager.GetTexture(): Failed to Create the Texture from File: %s, %s", DXGetErrorStringA(result), DXGetErrorDescriptionA(result));
			errorHandler->CreateFatalError(buffer);
			return nullptr;
		}



		// 2nd: Add it to the List
		textureResources.push_back(temp);

		// 3rd: Return the Texture
		return temp.texture;

	}
	else // No Texture loaded yet, so you can load this texture
	{
		TEXRESOURCE temp;
		temp.texname = textureName;
		temp.referenceCounter = 1;

		// 1st: Create the Texture
		result = D3DXCreateTextureFromFileExA(*d3dDevice,					// The Direct3DDevice.
											  temp.texname.c_str(),			// Name of the File.
											  D3DX_DEFAULT,					// Width of the Texture (Here: Use the Width of the Imagefile).
											  D3DX_DEFAULT,					// Height of the Texture (Here: Use the Width of the Imagefile).
											  1,							// Amount of MIP-Map-Layer (Here: Create as much as possible).
											  0,							// Usage of the Texture (Here: Static Texture).
											  D3DFMT_UNKNOWN,				// Dataformat of the Texture (Here: Use the Format of the Imagefile).
											  D3DPOOL_DEFAULT,				// How the Texture is handled (Default: You need to worry about
																			// Lost Devices, but you can use special functions).
											  D3DX_FILTER_NONE,				// Filter for Creating the Texture (Here: None).
											  D3DX_FILTER_NONE,				// Filter for Creating the Mip-Maps (Here: None).
											  0,							// ColorKey (The defined color is transparent!)
											  NULL,							// Always NULL
											  NULL,							// Only interesting for Pallettextures (8-Bit), otherwise NULL.
											  &temp.texture);				// Address to the Texture, that need to be created.
		if (FAILED(result))
		{
			char buffer[256];
			sprintf(buffer, "ResourceManager.GetTexture(): Failed to Create the Texture from File: %s, %s", DXGetErrorStringA(result), DXGetErrorDescriptionA(result));
			errorHandler->CreateFatalError(buffer);
			return nullptr;
		}

		// 2nd: Add it to the List
		textureResources.push_back(temp);

		// 3rd: Return the Texture
		return temp.texture;
	}
}
void ResourceManager::IntegrateExistingTexture(std::string textureName, LPDIRECT3DTEXTURE9 texture)
{
	if (texture == nullptr)
	{
		return;
	}

	// 1st: Create a TexResource
	TEXRESOURCE temp;
	temp.texture = texture;
	temp.texname = textureName;
	temp.referenceCounter = 1;

	// 2nd: Add it to the List
	textureResources.push_back(temp);

}
void ResourceManager::UnloadTexture(std::string textureName)
{
	int count = 0;

	for (std::vector<TEXRESOURCE>::iterator iterator = textureResources.begin(); iterator != textureResources.end(); ++iterator)
	{
		if (iterator->referenceCounter > 0 &&
			textureName == "Data/tiles.png")
		{
			int a = 10;
		}

		if (iterator->texname == textureName)
		{
			// This texture has already been loaded, first decrement the reference counter
			if (iterator->referenceCounter > 0)
			{
				iterator->referenceCounter--;
			}

			// If the counter is 0 or less, then you can unload it.
			if (iterator->referenceCounter == 0)
			{
				// 3.2.: Unload Texture
				iterator->texture->Release();
				iterator->texture = nullptr;
				ResourceManager::GetInstance()->textureResources.erase(iterator);
				break;
			}

		}
	}
}
void ResourceManager::ReleaseTextures()
{
	for (auto& entry : textureResources)
	{
		entry.texture->Release();
	}
}
void ResourceManager::ReloadTextures()
{
	HRESULT result; 
	for (auto& entry : textureResources)
	{
		int a = 10;
		// 1st: Create the Texture
		result = D3DXCreateTextureFromFileExA(*d3dDevice,					// The Direct3DDevice.
											  entry.texname.c_str(),		// Name of the File.
											  D3DX_DEFAULT,					// Width of the Texture (Here: Use the Width of the Imagefile).
											  D3DX_DEFAULT,					// Height of the Texture (Here: Use the Width of the Imagefile).
											  D3DX_DEFAULT,					// Amount of MIP-Map-Layer (Here: Create as much as possible).
											  0,							// Usage of the Texture (Here: Static Texture).
											  D3DFMT_UNKNOWN,				// Dataformat of the Texture (Here: Use the Format of the Imagefile).
											  D3DPOOL_DEFAULT,				// How the Texture is handled (Managed: You don't have to care
																			// about loosing access to your device (device lost). 
											  D3DX_FILTER_NONE,				// Filter for Creating the Texture (Here: None).
											  D3DX_DEFAULT,					// Filter for Creating the Mip-Maps (Here: None).
											  0,							// ColorKey (The defined color is transparent!)
											  NULL,							// Always NULL
											  NULL,							// Only interesting for Pallettextures (8-Bit), otherwise NULL.
											  &entry.texture);				// Address to the Texture, that need to be created.
		if (FAILED(result))
		{
			char buffer[256];
			sprintf(buffer, "ResourceManager.ReloadTexture(): Failed to Create the Texture from File: %s, %s", DXGetErrorStringA(result), DXGetErrorDescriptionA(result));
			errorHandler->CreateFatalError(buffer);
			return;
		}
	}
}

// Getters:
LPDIRECT3DTEXTURE9 ResourceManager::GetTexture(std::string textureName)
{
	if (textureResources.size() > 0)
	{
		// 1st: Check if this texture was already loaded
		for (auto& entry : textureResources)
		{
			if (entry.texname == textureName)
			{
				// Already loaded
				return entry.texture;
			}

		}
	}

	return nullptr;
}

ResourceManager* ResourceManager::instance = nullptr;