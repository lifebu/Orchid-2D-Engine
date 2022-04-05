//////////////////////////////////////////////////////////////////
// Orchid 2D Framework
// Version: Alpha 0.1
// File: Game.h
// Purpose: The Programmed Game!
//////////////////////////////////////////////////////////////////

// Preprocessor Directives:
#pragma once

// Define the DInput Version
#ifndef DIRECTINPUT_VERSION
#define DIRECTINPUT_VERSION 0x0800
#endif

// Includes:
#include <dinput.h>
#include <vector>
#include "core/system/Settings.h"
#include "core/System.h"
#include "renderer/camera/OrthoCamera.h"
#include "renderer/camera/Camera.h"
#include "renderer/sprite/Animated_Sprites.h"
#include "renderer/tilemap/Tileset.h"
#include "renderer/tilemap/Tile.h"
#include "renderer/tilemap/Tilelayer.h"
#include "renderer/tilemap/Tilemap.h"


/* The Actual Game.*/
class Game final
{
	// Functions:
	// Public functions:
public:
	Game(const SETTINGS& settings, const HWND hWnd, const HINSTANCE appInstance);
	~Game();

	/* False = Fatal Errors in the FrameCycle, abort. */
	bool FrameCycle();

	/* False = Fatal Errors while initializing, abort. */
	bool Initialize();
	void Shutdown();

	/* Change Data of the Objects accordingly to the Input */
	void HandleInput();

	// Private functions:
private:

	// Member:
	// Public Member:
public:
	// The Actual Framework!
	System system;

	// Private Member:
private:
	// Current Time passed between this and last frame.
	float deltaTime;

	// The systems ErrorHandler!
	ErrorHandler* errorHandler;

	// Settings
	const SETTINGS& settings;

	// Test Animated Sprites
	std::vector<Animated_Sprite> animSprite;

	// Test Tilemap
	Tilemap tilemap;

};

