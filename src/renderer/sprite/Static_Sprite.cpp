//////////////////////////////////////////////////////////////////
// Orchid 2D Framework
// Version: Alpha 0.1
// File: Static_Sprite.cpp
// Purpose: - This Class is used to render 2D Static Sprites with Hardware Acceleration.
//			- It's composed of 2 triangles, which are rendered.
//////////////////////////////////////////////////////////////////

// Preprocessor Directives:
#pragma once

// Includes
#include "Static_Sprite.h"

// Functions:
// Public functions:
Static_Sprite::Static_Sprite()
	:
	indexbuffer(nullptr),
	vertexbuffer(nullptr),
	isInitialized(false),
	position(position),
	rotation(0.0f, 0.0f, 0.0f),
	scaleX(1.0f),
	scaleY(1.0f),
	scaleZ(1.0f),
	textureFile(textureFile)

{
	tintcolor = D3DCOLOR_XRGB(255, 255, 255);

	D3DXMatrixTranslation(&translationmatrix, position.x, position.y, position.z);
	D3DXMatrixScaling(&scalingmatrix, scaleX, scaleY, scaleZ);
	D3DXMatrixRotationYawPitchRoll(&rotationmatrix, D3DXToRadian(rotation.x), D3DXToRadian(rotation.y), D3DXToRadian(rotation.z));
}
Static_Sprite::~Static_Sprite()
{
}

void Static_Sprite::Initialize(const LPDIRECT3DDEVICE9 d3dDevice, const D3DXVECTOR3 position,
	const std::string textureFile, const float minTexCoordX, const float minTexCoordY,
	const float maxTexCoordX, const float maxTexCoordY)
{
	// The Temporary Data:
	Vertex* vertices;
	WORD* indices;
	HRESULT result;
	ErrorHandler* errorHandler = ErrorHandler::GetInstance();

	// 1st: Set Member:
	this->position = position;
	D3DXMatrixTranslation(&translationmatrix, position.x, position.y, position.z);
	this->textureFile = textureFile;
	LPDIRECT3DTEXTURE9 tempTexture;

	//// 2nd: Create the Texture
	tempTexture = ResourceManager::GetInstance()->LoadTexture(textureFile);
	if (tempTexture == nullptr)
	{
		// There were problems loading the texture, so abort.
		return;
	}

	tempTexture->GetLevelDesc(0, &textureDesc);

	// 3rd: Create the Vertexbuffer
	result = d3dDevice->CreateVertexBuffer(4 * sizeof(Vertex),		// Size of the Vertexbuffer (Amount of Vertices * sizeof(one Vertex))
											0,						// Usage (Here: create a static Vertexbuffer).
											Vertex::FVF,			// The Vertexformat (use the preexisting definition)
											D3DPOOL_MANAGED,		// How the Vertexbuffer is handled (Managed: You don't have to care
																	// about loosing access to your device (device lost).
											&vertexbuffer,		// Address of the Buffer which shall be created.
											nullptr);				// Unimportant, use nullptr!
	if (FAILED(result))
	{
		char buffer[256];
		sprintf(buffer, "Sprite.Initialize(): Failed to Create a Vertexbuffer: %s, %s", DXGetErrorStringA(result), DXGetErrorDescriptionA(result));
		errorHandler->CreateFatalError(buffer);
		return;
	}

	// 4th: Create Indexbuffer
	result = d3dDevice->CreateIndexBuffer(6 * sizeof(WORD),		// Size of the Indexbuffer (Amount of Indices * sizeof(one Index))
										   0,						// Usage (Here: create a static Vertexbuffer).
										   D3DFMT_INDEX16,			// Use 16-Bit Indexbuffers (more than enough here)
										   D3DPOOL_MANAGED,			// How the Indexbuffer is handled (Managed: You don't have to care
																	// about loosing access to your device (device lost).
										   &indexbuffer,			// Address of the Buffer which shall be created.			
										   nullptr);				// Unimportant, use nullptr!
	if (FAILED(result))
	{
		char buffer[256];
		sprintf(buffer, "Sprite.Initialize(): Failed to Create a Indexbuffer: %s, %s", DXGetErrorStringA(result), DXGetErrorDescriptionA(result));
		errorHandler->CreateFatalError(buffer);
		return;
	}

	// 5th: Fill Buffers
	// 5.1.: Lock the Buffers
	result = vertexbuffer->Lock(0,								// Offset for locking (0: from the beginning of the buffer)
								  0,								// The Size to lock (0: lock entire buffer)
								  (void**)(&vertices),				// The Array-Pointer, from which we will gain access to the data
								  D3DLOCK_NOSYSLOCK);				// How it shall lock it (Here: Let the System work while the buffer is locked)
	if (FAILED(result))
	{
		char buffer[256];
		sprintf(buffer, "Sprite.Initialize(): Failed to Lock the Vertexbuffer: %s, %s", DXGetErrorStringA(result), DXGetErrorDescriptionA(result));
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
		sprintf(buffer, "Sprite.Initialize(): Failed to Lock the Indexbuffer: %s, %s", DXGetErrorStringA(result), DXGetErrorDescriptionA(result));
		errorHandler->CreateFatalError(buffer);
		return;
	}

	// 5.2. Set the Position of the vertices
	if (textureFile == "Data/tiles.png")
	{
		int a = 10;
	}
	vertices[0].position = D3DXVECTOR3(-(((float)textureDesc.Width * (maxTexCoordX - minTexCoordX)) / 2.0f),
		(((float)textureDesc.Height * (maxTexCoordY - minTexCoordY)) / 2.0f), 0.0f);
	vertices[1].position = D3DXVECTOR3((((float)textureDesc.Width * (maxTexCoordX - minTexCoordX)) / 2.0f),
		(((float)textureDesc.Height * (maxTexCoordY - minTexCoordY)) / 2.0f), 0.0f);
	vertices[2].position = D3DXVECTOR3((((float)textureDesc.Width * (maxTexCoordX - minTexCoordX)) / 2.0f),
		-(((float)textureDesc.Height * (maxTexCoordY - minTexCoordY)) / 2.0f), 0.0f);
	vertices[3].position = D3DXVECTOR3(-(((float)textureDesc.Width * (maxTexCoordX - minTexCoordX)) / 2.0f),
		-(((float)textureDesc.Height * (maxTexCoordY - minTexCoordY)) / 2.0f), 0.0f);



	// 5.3. Set the Color of the vertices:
	vertices[0].diffuseColor = D3DCOLOR_ARGB(255, 255, 255, 255);
	vertices[1].diffuseColor = D3DCOLOR_ARGB(255, 255, 255, 255);
	vertices[2].diffuseColor = D3DCOLOR_ARGB(255, 255, 255, 255);
	vertices[3].diffuseColor = D3DCOLOR_ARGB(255, 255, 255, 255);

	// 5.4. Set the Texture coordinates of the vertices:
	vertices[0].texture = D3DXVECTOR2(minTexCoordX, minTexCoordY);
	vertices[1].texture = D3DXVECTOR2(maxTexCoordX, minTexCoordY);
	vertices[2].texture = D3DXVECTOR2(maxTexCoordX, maxTexCoordY);
	vertices[3].texture = D3DXVECTOR2(minTexCoordX, maxTexCoordY);


	// 5.5. Create the Indices
	int Index[6] = { 0, 1, 2,
		             0, 2, 3 };

	for (int i = 0; i < 6; i++)
	{
		indices[i] = (WORD)(Index[i]);
	}

	// 6th: Unlock Vertex and Indexbufer
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

	// 7th: The Sprite was successfully initialized
	isInitialized = true;
	return;
}

void Static_Sprite::Shutdown()
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
	// 3rd: Release Texture if initialized.
	ResourceManager::GetInstance()->UnloadTexture(textureFile);
	textureFile = "";

	// 4th: It's not initialized anymore!
	isInitialized = false;
	return;
}

bool Static_Sprite::IsInitialized() const
{
	return isInitialized;
}
void Static_Sprite::RotateBy(D3DXVECTOR3 amount)
{
	// 1st: Add to the current rotation
	rotation += amount;
	
	// 2nd: Check if the Values are greater than 360 (Degrees) and bound them.
	if (rotation.x == 360.0f)
	{
		rotation.x = 0.0f;
	}
	else if (rotation.x > 360.0f)
	{
		rotation.x = rotation.x - 360;
	}
	if (rotation.y == 360.0f)
	{
		rotation.y = 0.0f;
	}
	else if (rotation.y > 360.0f)
	{
		rotation.y = rotation.y - 360;
	}
	if (rotation.z == 360.0f)
	{
		rotation.z = 0.0f;
	}
	else if (rotation.z > 360.0f)
	{
		rotation.z = rotation.z - 360;
	}

	// 3rd: Update RotationMatrix.
	D3DXMatrixRotationYawPitchRoll(&rotationmatrix, D3DXToRadian(rotation.x), D3DXToRadian(rotation.y), D3DXToRadian(rotation.z));
}
void Static_Sprite::ScaleBy(float amount)
{
	// 1st: Add the amount to the current Scale
	scaleX += amount;
	scaleY += amount;
	scaleZ += amount;

	// 2nd: Update the Scalingmatrix accordingly.
	D3DXMatrixScaling(&scalingmatrix, scaleX, scaleY, scaleZ);
}
void Static_Sprite::TranslateBy(D3DXVECTOR3 amount)
{
	// 1st: Add the amount to the current positon.
	position += amount;

	// 2nd: Update the Translationmatrix accordingly.
	D3DXMatrixTranslation(&translationmatrix, position.x, position.y, position.z);
}

// Getters:
const LPDIRECT3DVERTEXBUFFER9 Static_Sprite::GetVertexBuffer() const
{
	return vertexbuffer;
}
const LPDIRECT3DINDEXBUFFER9 Static_Sprite::GetIndexBuffer() const
{
	return indexbuffer;
}
const LPDIRECT3DTEXTURE9 Static_Sprite::GetTexture() const
{
	return ResourceManager::GetInstance()->GetTexture(textureFile);
}
const D3DXVECTOR3& Static_Sprite::GetPosition() const
{
	return position;
}
const D3DXVECTOR3& Static_Sprite::GetRotation() const
{
	return rotation;
}
const float& Static_Sprite::GetScaleX() const
{
	return scaleX;
}
const float& Static_Sprite::GetScaleY() const
{
	return scaleY;
}
const float& Static_Sprite::GetScaleZ() const
{
	return scaleZ;
}
const UINT& Static_Sprite::GetHeight() const
{
	return textureDesc.Height;
}
const UINT& Static_Sprite::GetWidth() const
{
	return textureDesc.Width;
}
const D3DCOLOR& Static_Sprite::GetColor() const
{
	return tintcolor;
}
const D3DXMATRIX& Static_Sprite::GetRotationMatrix() const
{
	return rotationmatrix;
}
const D3DXMATRIX& Static_Sprite::GetTranslationMatrix() const
{
	return translationmatrix;
}
const D3DXMATRIX& Static_Sprite::GetScalingMatrix() const
{
	return scalingmatrix;
}

// Setters
void Static_Sprite::SetColor(D3DCOLOR newColor)
{
	tintcolor = newColor;
}
void Static_Sprite::SetPosition(D3DXVECTOR3 newPos)
{
	position = newPos;
	
	// Update TranslationMatrix.
	D3DXMatrixTranslation(&translationmatrix, position.x, position.y, position.z);
}
void Static_Sprite::SetRotation(D3DXVECTOR3 newRotation)
{
	rotation = newRotation;
	// Check if the Values are greater than 360 (Degrees) and bound them.
	if (rotation.x == 360.0f)
	{
		rotation.x = 0.0f;
	}
	else if (rotation.x > 360.0f)
	{
		rotation.x = rotation.x - 360;
	}
	if (rotation.y == 360.0f)
	{
		rotation.y = 0.0f;
	}
	else if (rotation.y > 360.0f)
	{
		rotation.y = rotation.y - 360;
	}
	if (rotation.z == 360.0f)
	{
		rotation.z = 0.0f;
	}
	else if (rotation.z > 360.0f)
	{
		rotation.z = rotation.z - 360;
	}

	// Update RotationMatrix.
	D3DXMatrixRotationYawPitchRoll(&rotationmatrix, D3DXToRadian(rotation.x), D3DXToRadian(rotation.y), D3DXToRadian(rotation.z));
}
void Static_Sprite::SetScaleX(float newScale)
{
	scaleX = newScale;

	// Update ScaleMatrix.
	D3DXMatrixScaling(&scalingmatrix, scaleX, scaleY, scaleZ);
}
void Static_Sprite::SetScaleY(float newScale)
{
	scaleY = newScale;

	// Update ScaleMatrix.
	D3DXMatrixScaling(&scalingmatrix, scaleX, scaleY, scaleZ);
}
void Static_Sprite::SetScaleZ(float newScale)
{
	scaleZ = newScale;

	// Update ScaleMatrix.
	D3DXMatrixScaling(&scalingmatrix, scaleX, scaleY, scaleZ);
}