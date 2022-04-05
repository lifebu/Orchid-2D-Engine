//////////////////////////////////////////////////////////////////
// Orchid 2D Framework
// Version: Alpha 0.1
// File: Animated_Sprites.cpp
// Purpose: - This Class is used to render 2D Animated Sprites with Hardware Acceleration.
//			- It's composed of 2 triangles, which are rendered.
//////////////////////////////////////////////////////////////////

// Includes
#include "Animated_Sprites.h"

// Functions:
// Public functions:
Animated_Sprite::Animated_Sprite()
	:
	Static_Sprite(),
	isPlaying(false),
	loop(false),
	numFrames(1),
	frameWidth(1),
	frameHeight(1),
	currentFrame(0),
	isFlipped(false)
{
	tintcolor = D3DCOLOR_XRGB(255, 255, 255);

	D3DXMatrixTranslation(&translationmatrix, position.x, position.y, position.z);
	D3DXMatrixScaling(&scalingmatrix, scaleX, scaleY, scaleZ);
	D3DXMatrixRotationYawPitchRoll(&rotationmatrix, D3DXToRadian(rotation.x), D3DXToRadian(rotation.y), D3DXToRadian(rotation.z));
}
Animated_Sprite::~Animated_Sprite()
{

}

void Animated_Sprite::Initialize(const LPDIRECT3DDEVICE9 d3dDevice, const D3DXVECTOR3 position,
	const std::string textureFile, const int numFrames, const int frameWidth,
	const int frameHeight)
{
	// The Temporary Data:
	Vertex* vertices;
	WORD* indices;
	HRESULT result;
	ErrorHandler* errorHandler = ErrorHandler::GetInstance();



	// 1st: Set Member:
	this->position = position;
	D3DXMatrixTranslation(&translationmatrix, position.x, position.y, position.z);
	this->frameWidth = frameWidth;
	this->frameHeight = frameHeight;
	this->numFrames = numFrames;
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
										   Vertex::FVF,				// The Vertexformat (use the preexisting definition)
										   D3DPOOL_MANAGED,			// How the Vertexbuffer is handled (Managed: You don't have to care
																	// about loosing access to your device (device lost).
										   &vertexbuffer,			// Address of the Buffer which shall be created.
										   nullptr);				// Unimportant, use nullptr!
	if (FAILED(result))
	{
		char buffer[256];
		sprintf(buffer, "Sprite.Initialize(): Failed to Create a Vertexbuffer: %s, %s", DXGetErrorStringA(result), DXGetErrorDescriptionA(result));
		errorHandler->CreateFatalError(buffer);
		return;
	}

	// 4th: Create Indexbuffer
	result = d3dDevice->CreateIndexBuffer(6 * sizeof(WORD),		    // Size of the Indexbuffer (Amount of Indices * sizeof(one Index))
										  0,						// Usage (Here: create a static Vertexbuffer).
										  D3DFMT_INDEX16,			// Use 16-Bit Indexbuffers (more than enough here)
										  D3DPOOL_MANAGED,			// How the Indexbuffer is handled (Managed: You don't have to care
																	// about loosing access to your device (device lost).
										  &indexbuffer,			// Address of the Buffer which shall be created.			
										  nullptr);				    // Unimportant, use nullptr!
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

	vertices[0].position = D3DXVECTOR3(-(float)(frameWidth / 2), (float)(frameHeight / 2), 0.0f);
	vertices[1].position = D3DXVECTOR3((float)(frameWidth / 2), (float)(frameHeight / 2), 0.0f);
	vertices[2].position = D3DXVECTOR3((float)(frameWidth / 2), -(float)(frameHeight / 2), 0.0f);
	vertices[3].position = D3DXVECTOR3(-(float)(frameWidth / 2), -(float)(frameHeight / 2), 0.0f);


	// 5.3. Set the Color of the vertices:
	vertices[0].diffuseColor = D3DCOLOR_ARGB(255, 255, 255, 255);
	vertices[1].diffuseColor = D3DCOLOR_ARGB(255, 255, 255, 255);
	vertices[2].diffuseColor = D3DCOLOR_ARGB(255, 255, 255, 255);
	vertices[3].diffuseColor = D3DCOLOR_ARGB(255, 255, 255, 255);

	// 5.4. Set the Texture coordinates of the vertices:
	// 5.4.1. Set the Number of Frames per Row:
	numFramesPerRow = textureDesc.Width / frameWidth;

	vertices[0].texture = D3DXVECTOR2(0, 0);
	vertices[1].texture = D3DXVECTOR2(1.0f / numFramesPerRow, 0);
	vertices[2].texture = D3DXVECTOR2(1.0f / numFramesPerRow, 1.0f / numFramesPerRow);
	vertices[3].texture = D3DXVECTOR2(0, 1.0f / numFramesPerRow);

	

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


// Animation functions:
bool Animated_Sprite::IsPlaying()
{
	return isPlaying;
}
void Animated_Sprite::Play(bool loop)
{
	loop = true;
	isPlaying = true;
}
void Animated_Sprite::Update()
{
	// Temp Data:
	ErrorHandler* errorHandler = ErrorHandler::GetInstance();

	if (isInitialized)
	{
		if (isPlaying)
		{
			if (TimeManager::GetInstance()->IsWatchtimeReached("AnimFreq"))
			{
				HRESULT result;
				Vertex* vertices;
				float minTextureCoordX;
				float maxTextureCoordX;
				float minTextureCoordY;
				float maxTextureCoordY;

				result = vertexbuffer->Lock(0, 0, (void**)(&vertices), D3DLOCK_NOSYSLOCK);
				if (FAILED(result))
				{
					char buffer[256];
					sprintf(buffer, "Animated_Sprite.Update(): Failed to Lock the Vertexbuffer: %s, %s", DXGetErrorStringA(result), DXGetErrorDescriptionA(result));
					errorHandler->CreateFatalError(buffer);
					return;
				}

				currentFrame++;
				if (currentFrame > (numFrames - 1))
				{
					currentFrame = 0;
					if (!loop)
					{
						isPlaying = false;
						loop = false;
					}
				}



				float FrameTextureWidth = 1.0f / (float)(textureDesc.Width / frameWidth);
				float FrameTextureHeight = 1.0f / (float)(textureDesc.Height / frameHeight);

				minTextureCoordX = (1.0f / (float)numFramesPerRow) * (float)(currentFrame % numFramesPerRow);
				maxTextureCoordX = minTextureCoordX + FrameTextureWidth;
				minTextureCoordY = (1.0f / (float)numFramesPerRow) * (float)(currentFrame / numFramesPerRow);
				maxTextureCoordY = minTextureCoordY + FrameTextureHeight;

				if (!isFlipped)
				{
					vertices[0].texture = D3DXVECTOR2(minTextureCoordX, minTextureCoordY);
					vertices[1].texture = D3DXVECTOR2(maxTextureCoordX, minTextureCoordY);
					vertices[2].texture = D3DXVECTOR2(maxTextureCoordX, maxTextureCoordY);
					vertices[3].texture = D3DXVECTOR2(minTextureCoordX, maxTextureCoordY);
				}
				else
				{
					vertices[1].texture = D3DXVECTOR2(minTextureCoordX, minTextureCoordY);
					vertices[0].texture = D3DXVECTOR2(maxTextureCoordX, minTextureCoordY);
					vertices[3].texture = D3DXVECTOR2(maxTextureCoordX, maxTextureCoordY);
					vertices[2].texture = D3DXVECTOR2(minTextureCoordX, maxTextureCoordY);
				}

				// 6th: Unlock Vertexbuffer
				result = vertexbuffer->Unlock();
				if (FAILED(result))
				{
					char buffer[256];
					sprintf(buffer, "Animated_Sprite.Update(): Failed to Unlock the Vertexbuffer: %s, %s", DXGetErrorStringA(result), DXGetErrorDescriptionA(result));
					errorHandler->CreateFatalError(buffer);
					return;
				}
			}
		}
	}
}
void Animated_Sprite::Pause()
{
	isPlaying = false;
}
void Animated_Sprite::Stop()
{

}

// Getters:
const bool& Animated_Sprite::GetLoop() const
{
	return loop;
}
const bool& Animated_Sprite::GetFlip() const
{
	return isFlipped;
}

// Setters
void Animated_Sprite::SetLoop(bool newValue)
{
	loop = newValue;
}
void Animated_Sprite::SetFlip(bool newValue)
{
	isFlipped = newValue;
}