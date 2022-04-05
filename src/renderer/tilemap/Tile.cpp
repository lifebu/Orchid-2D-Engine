//////////////////////////////////////////////////////////////////
// Orchid 2D Framework
// Version: Alpha 0.1
// File: Tile.cpp
// Purpose: - This Class is a one single tile
//			- 
//////////////////////////////////////////////////////////////////

// Includes
#include "Tile.h"

// Functions:
// Public functions:
Tile::Tile()
{

}
Tile::~Tile()
{

}

void Tile::Initialize(const LPDIRECT3DDEVICE9 d3dDevice, const D3DXVECTOR3 position, const UINT GID,
	const Tileset& tileset, UINT tileWidth, UINT tileHeight)
{
	// The Temporary Data:
	Vertex* vertices;
	WORD* indices;
	HRESULT result;
	ErrorHandler* errorHandler = ErrorHandler::GetInstance();

	// 1st: Set Member:
	this->position = position;
	D3DXMatrixTranslation(&translationmatrix, position.x, position.y, position.z);
	this->GID = GID;

	// 2nd: Create the Vertexbuffer
	result = d3dDevice->CreateVertexBuffer(4 * sizeof(Vertex),		// Size of the Vertexbuffer (Amount of Vertices * sizeof(one Vertex))
										  0,						// Usage (Here: create a static Vertexbuffer).
										  Vertex::FVF,				// The Vertexformat (use the preexisting definition)
										  D3DPOOL_MANAGED,			// How the Vertexbuffer is handled (Managed: You don't have to care
																	// about loosing access to your device (device lost).
										  &vertexbuffer,			// Address of the Buffer which shall be created.
										  nullptr);					// Unimportant, use nullptr!
	if (FAILED(result))
	{
		char buffer[256];
		sprintf(buffer, "Tile.Initialize(): Failed to Create a Vertexbuffer: %s, %s", DXGetErrorStringA(result), DXGetErrorDescriptionA(result));
		errorHandler->CreateFatalError(buffer);
		return;
	}

	// 3rd: Create Indexbuffer
	result = d3dDevice->CreateIndexBuffer(6 * sizeof(WORD),			// Size of the Indexbuffer (Amount of Indices * sizeof(one Index))
										  0,						// Usage (Here: create a static Vertexbuffer).
										  D3DFMT_INDEX16,			// Use 16-Bit Indexbuffers (more than enough here)
										  D3DPOOL_MANAGED,			// How the Indexbuffer is handled (Managed: You don't have to care
																	// about loosing access to your device (device lost).
										  &indexbuffer,			// Address of the Buffer which shall be created.			
										  nullptr);					// Unimportant, use nullptr!
	if (FAILED(result))
	{
		char buffer[256];
		sprintf(buffer, "Tile.Initialize(): Failed to Create a Indexbuffer: %s, %s", DXGetErrorStringA(result), DXGetErrorDescriptionA(result));
		errorHandler->CreateFatalError(buffer);
		return;
	}

	// 4th: Fill Buffers
	// 4.1.: Lock the Buffers
	result = vertexbuffer->Lock(0,								// Offset for locking (0: from the beginning of the buffer)
								  0,								// The Size to lock (0: lock entire buffer)
								  (void**)(&vertices),				// The Array-Pointer, from which we will gain access to the data
								  D3DLOCK_NOSYSLOCK);				// How it shall lock it (Here: Let the System work while the buffer is locked)
	if (FAILED(result))
	{
		char buffer[256];
		sprintf(buffer, "Tile.Initialize(): Failed to Lock the Vertexbuffer: %s, %s", DXGetErrorStringA(result), DXGetErrorDescriptionA(result));
		errorHandler->CreateFatalError(buffer);
		return;
	}
	result = indexbuffer->Lock(0,									// Offset for locking (0: from the beginning of the buffer)
								 0,									// The Size to lock (0: lock entire buffer)
								 (void**)(&indices),				// The Array-Pointer, from which we will gain access to the data
								 D3DLOCK_NOSYSLOCK);				// How it shall lock it (Here: Let the System work while the buffer is locked)
	if (FAILED(result))
	{
		char buffer[256];
		sprintf(buffer, "Tile.Initialize(): Failed to Lock the Indexbuffer: %s, %s", DXGetErrorStringA(result), DXGetErrorDescriptionA(result));
		errorHandler->CreateFatalError(buffer);
		return;
	}

	// 4.2. Set the Position of the vertices
	vertices[0].position = D3DXVECTOR3(-((float)tileWidth / 2.0f),
		((float)tileHeight / 2.0f), 0.0f);
	vertices[1].position = D3DXVECTOR3(((float)tileWidth / 2.0f),
		((float)tileHeight / 2.0f), 0.0f);
	vertices[2].position = D3DXVECTOR3(((float)tileWidth / 2.0f),
		-((float)tileHeight / 2.0f), 0.0f);
	vertices[3].position = D3DXVECTOR3(-((float)tileWidth / 2.0f),
		-((float)tileHeight / 2.0f), 0.0f);


	// 4.3. Set the Color of the vertices:
	vertices[0].diffuseColor = D3DCOLOR_ARGB(255, 255, 255, 255);
	vertices[1].diffuseColor = D3DCOLOR_ARGB(255, 255, 255, 255);
	vertices[2].diffuseColor = D3DCOLOR_ARGB(255, 255, 255, 255);
	vertices[3].diffuseColor = D3DCOLOR_ARGB(255, 255, 255, 255);

	// 4.4. Set the Texture coordinates of the vertices:
	float minTexCoordX = 0.0f;
	float maxTexCoordX = 1.0f;
	float minTexCoordY = 0.0f;
	float maxTexCoordY = 1.0f;
	tileset.GetTextureCoords(GID, minTexCoordX, maxTexCoordX, minTexCoordY, maxTexCoordY);
	vertices[0].texture = D3DXVECTOR2(minTexCoordX, minTexCoordY);
	vertices[1].texture = D3DXVECTOR2(maxTexCoordX, minTexCoordY);
	vertices[2].texture = D3DXVECTOR2(maxTexCoordX, maxTexCoordY);
	vertices[3].texture = D3DXVECTOR2(minTexCoordX, maxTexCoordY);


	// 4.5. Create the Indices
	int Index[6] = { 0, 1, 2,
		0, 2, 3 };

	for (int i = 0; i < 6; i++)
	{
		indices[i] = (WORD)(Index[i]);
	}

	// 5th: Unlock Vertex and Indexbufer
	result = vertexbuffer->Unlock();
	if (FAILED(result))
	{
		char buffer[256];
		sprintf(buffer, "Sprite.Initialize(): Failed to Unlock the Vertexbuffer: %s, %s", DXGetErrorStringA(result), DXGetErrorDescriptionA(result));
		errorHandler->CreateFatalError(buffer);
		return;
	}
	result = indexbuffer->Unlock();
	if (FAILED(result))
	{
		char buffer[256];
		sprintf(buffer, "Sprite.Initialize(): Failed to Unlock the Indexbuffer: %s, %s", DXGetErrorStringA(result), DXGetErrorDescriptionA(result));
		errorHandler->CreateFatalError(buffer);
		return;
	}

	// 6th: The Sprite was successfully initialized!
	isInitialized = true;
	return;
}
void Tile::Shutdown()
{
	// 1st: Release Vertexbuffer if initialized.
	if (vertexbuffer != nullptr)
	{
		vertexbuffer->Release();
		vertexbuffer = nullptr;
	}
	// 2nd: Release Indexbuffer if initialized.
	if (indexbuffer != nullptr)
	{
		indexbuffer->Release();
		indexbuffer = nullptr;
	}
}

// Getters:
const UINT& Tile::GetGID() const
{
	return GID;
}
const D3DXVECTOR3& Tile::GetPosition() const
{
	return position;
}

// Setters:
void Tile::SetGID(UINT newGID)
{
	GID = newGID;
}
void Tile::SetPosition(D3DXVECTOR3 NewPos)
{
	position = NewPos;
}

bool Tile::IsInitialized() const
{
	return isInitialized;
}