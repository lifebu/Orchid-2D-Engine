//////////////////////////////////////////////////////////////////
// Orchid 2D Framework
// Version: Alpha 0.1
// File: Graphics.h
// Purpose: - This Class is responsible for the Graphic.
//			- It uses Direct3D 9.0c.
//////////////////////////////////////////////////////////////////

// Preprocessor Directives:
#pragma once

// Libraries
#pragma comment(lib, "d3d9.lib")
#pragma comment(lib, "d3dx9.lib")
#pragma comment(lib, "dxerr.lib")

// Includes
#include "core/system/ErrorHandler.h"
#include "core/system/Settings.h"
#include "renderer/camera/OrthoCamera.h"
#include "renderer/sprite/Static_Sprite.h"
#include "renderer/GraphicDefinitions.h"
#include <d3d9.h>
#include <d3dx9.h>
#include <DxErr.h>
#include <cassert>
#include "renderer/sprite/Animated_Sprites.h"
#include "renderer/tilemap/Tile.h"
#include "renderer/tilemap/Tileset.h"
#include "renderer/tilemap/Tilemap.h"
#include "renderer/font/Font.h"

class Graphics final
{
	// Functions:
	// Public functions:
public:
	Graphics(const SETTINGS& Settings, const HWND hWnd);
	~Graphics();

	/* Initialize the Graphic. */
	void Initialize();
	/* Shutdown the Graphic. */
	void Shutdown();

	/* Begin the Drawing Part of the Frame. */
	void BeginFrame();
	/* End the Drawing Part of the Frame. */
	void EndFrame();

	/* Render a Sprite. */
	void RenderSprite(const Static_Sprite& sprite);
	void RenderSprite(const Animated_Sprite& sprite);
	void RenderTilemap(const Tilemap& tilemap);
	void RenderTileLayers(const std::vector<Tilelayer>& tilelayers, const std::vector<Tileset>& tilesets, const Tilemap& tilemap, 
		const UINT& tileXStart, const UINT& tileXEnd, const UINT& tileYStart, const UINT& tileYEnd);
	void RenderImageLayers(const std::vector<Static_Sprite>& imageLayer);
	void RenderObjectLayers(const std::vector<Animated_Sprite*>& objectLayer);

	// Getters
	LPDIRECT3DDEVICE9* GetDevice();
	OrthogonalCamera& GetActiveCamera();
	OrthogonalCamera& GetDebugCamera();

	// Setters
	void SetCamera(OrthogonalCamera& camera);

	// Font Stuff
	void InitializeFont(const std::string& font, const DWORD& fontSize, const bool& italic, const bool& bold);
	void DrawText(const std::string string, const int& x, const int& y, const D3DCOLOR& color);

	// Camera Functions:
	void ToggleDebugCam();
	BOOL isDebugCamActive();

	// Private functions:
private:
	void SetDeviceStandarts();

	// Member:
	// Public Member:
public:

	// Private Member:
private:
	/* Settings*/
	const SETTINGS& settings;
	/* Window Handle. */
	const HWND hWnd;

	// DirectX Interfaces
	/* Direct3D Interface: */
	LPDIRECT3D9 direct3D;
	/* Device Interface: */
	LPDIRECT3DDEVICE9 d3dDevice;
	/* Present Paramenters*/
	D3DPRESENT_PARAMETERS presentParameters;
	/* Texture which is actively used atm. */
	LPDIRECT3DTEXTURE9 activeTexture;

	// 3D Data
	/* Active Camera. */
	OrthogonalCamera activeCamera;
	/* Debug Camera. */
	OrthogonalCamera debugCamera;
	BOOL useDebugCam;
	/* The Font. */
	Font font;
	/* Queue for drawing the Texts. */
	std::vector<FontDraw> TextDrawQueue;

	// Debug Data:
	/* Do you want to draw Debug Information? */
	bool drawDebugInfo;
	/* How many tiles have been drawn? */
	UINT renderedTiles;
	/* The area of the active Camera. */
	Static_Sprite areaActiveCamera;
	/* ErrorHandler: */
	ErrorHandler* errorHandler;


};
