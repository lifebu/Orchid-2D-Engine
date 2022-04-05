//////////////////////////////////////////////////////////////////
// Orchid 2D Framework
// Version: Alpha 0.1
// File: Static_Sprite.h
// Purpose: - This Class is used to render 2D Static Sprites with Hardware Acceleration.
//			- It's composed of 2 triangles, which are rendered.
//////////////////////////////////////////////////////////////////

// Preprocessor Directives:
#pragma once



// Includes
#include <d3dx9.h>
#include <DxErr.h>
#include <string>
#include "core/system/ErrorHandler.h"
#include "renderer/GraphicDefinitions.h"
#include "core/system/resources/ResourceManager.h"


class Static_Sprite 
{
	// Functions:
	// Public functions:
public:
	Static_Sprite();
public:
	~Static_Sprite();

	/* Initialize the Static Sprite. */
	void Initialize(const LPDIRECT3DDEVICE9 d3dDevice, const D3DXVECTOR3 position, 
		const std::string textureFile, const float minTexCoordX = 0.0f, const float minTexCoordY = 0.0f,
		const float maxTexCoordX = 1.0f, const float maxTexCoordY = 1.0f);
	/* Shutdown the Static Sprite. */
	void Shutdown();
	
	bool IsInitialized() const;
	void RotateBy(D3DXVECTOR3 amount);
	void ScaleBy(float amount);
	void TranslateBy(D3DXVECTOR3 amount);

	// Getters:
	const LPDIRECT3DVERTEXBUFFER9 GetVertexBuffer() const;
	const LPDIRECT3DINDEXBUFFER9 GetIndexBuffer() const;
	const LPDIRECT3DTEXTURE9 GetTexture() const;
	const D3DXVECTOR3& GetPosition() const;
	const D3DXVECTOR3& GetRotation() const;
	const float& GetScaleX() const;
	const float& GetScaleY() const;
	const float& GetScaleZ() const;
	const UINT& GetHeight() const;
	const UINT& GetWidth() const;
	const D3DCOLOR& GetColor() const;
	const D3DXMATRIX& GetRotationMatrix() const;
	const D3DXMATRIX& GetTranslationMatrix() const;
	const D3DXMATRIX& GetScalingMatrix() const;

	// Setters
	void SetColor(D3DCOLOR newColor);
	void SetPosition(D3DXVECTOR3 newPos);
	void SetRotation(D3DXVECTOR3 newRotation);
	void SetScaleX(float newScale);
	void SetScaleY(float newScale);
	void SetScaleZ(float newScale);

	// Private functions:
private:

	// Member:
	// Public Member:
public:

	// Private Member:
protected:
	// Position
	D3DXVECTOR3 position;
	// TintColor
	D3DCOLOR tintcolor;
	// Rotation
	/* X-Value = Rotation around global X-Axis. */
	/* Y-Value = Rotation around global Y-Axis. */
	/* Z-Value = Rotation around global Z-Axis. */
	D3DXVECTOR3 rotation;
	// Scale
	float scaleX;
	float scaleY;
	float scaleZ;
	// Description of the Texure.
	D3DSURFACE_DESC textureDesc;

	D3DXMATRIX scalingmatrix;
	D3DXMATRIX rotationmatrix;
	D3DXMATRIX translationmatrix;

	// Buffers
	LPDIRECT3DVERTEXBUFFER9 vertexbuffer;
	LPDIRECT3DINDEXBUFFER9 indexbuffer;
	/*LPDIRECT3DTEXTURE9 texture;*/

	// Is the Sprite initialized?
	bool isInitialized;

	std::string textureFile;
	

};