//////////////////////////////////////////////////////////////////
// Orchid 2D Framework
// Version: Alpha 0.1
// File: Tile.h
// Purpose: - This Class is a one single tile
//			- 
//////////////////////////////////////////////////////////////////

// Preprocessor Directives:
#pragma once

// Includes
#include <string>
#include <vector>
#include <d3dx9.h>
#include "core/system/ErrorHandler.h"
#include "core/system/resources/ResourceManager.h"
#include "renderer/sprite/Static_Sprite.h"
#include "Tileset.h"

/* Tileset Class. */
class Tile : public Static_Sprite
{
	// Functions:
	// Public functions:
public:
	Tile();
	~Tile();

	/* Initialize the Tile. */
	void Initialize(const LPDIRECT3DDEVICE9 d3dDevice, const D3DXVECTOR3 position, const UINT GID, 
		const Tileset& tileset, UINT tileWidth, UINT tileHeight);
	/* Shutdown the Tile */
	void Shutdown();

	// Getters:
	const UINT& GetGID() const;
	const D3DXVECTOR3& GetPosition() const;

	// Setters:
	void SetGID(UINT newGID);
	void SetPosition(D3DXVECTOR3 NewPos);

	bool IsInitialized() const;

	// Not defined functions from static_sprite uninteresting for a tile:
	void RotateBy(D3DXVECTOR3 amount);
	void ScaleBy(float amount);
	void TranslateBy(D3DXVECTOR3 amount);
	const LPDIRECT3DTEXTURE9 GetTexture() const;
	const D3DXVECTOR3& GetRotation() const;
	const float& GetScale() const;
	const UINT& GetHeight() const;
	const UINT& GetWidth() const;
	const D3DCOLOR& GetColor() const;
	void SetColor(D3DCOLOR newColor);
	void SetRotation(D3DXVECTOR3 newRotation);
	void SetScale(float newScale);

	// Private functions:
private:

	// Member:
	// Public Member:
public:

	// Private Member:
private:
	/* GID to know which texture is used.
	 * 0 = no Texture here. */
	UINT GID;

};