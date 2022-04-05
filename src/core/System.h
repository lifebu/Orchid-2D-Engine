//////////////////////////////////////////////////////////////////
// Orchid 2D Framework
// Version: Alpha 0.1
// File: System.h
// Purpose: The Systemclass for the Framework
//		    Holding together all the Subsystems and important
//          System Components!
//////////////////////////////////////////////////////////////////

// Preprocessor Directives:
#pragma once

// Includes:
#include <windows.h>
#include <d3d9types.h>
#include "core/system/Settings.h"
#include "core/system/ErrorHandler.h"
#include "core/system/resources/ResourceManager.h"
#include "core/system/time/TimeManager.h"
#include "core/system/Profiler.h"
#include "Graphics.h"
#include "Input.h"
#include "Physics.h"
#include "Sound.h"
#include "renderer/camera/Camera.h"
#include "renderer/sprite/Static_Sprite.h"
#include "renderer/sprite/Animated_Sprites.h"
#include "renderer/tilemap/Tilemap.h"
#include "renderer/Transform.h"


/* The Framework system, that holds together the functions and subsystems.*/
class System final
{
	// Functions:
	// Public functions:
public:
	System(const SETTINGS& settings, const HWND hWnd, const HINSTANCE appInstance);
	~System();

	void FrameCycle(const Tilemap& tilemap);

	/* False = Fatal Errors while initializing, abort. */
	bool Initialize();
	void Shutdown();

	/* Prints all Errors. False = Fatal Errors Detected.*/
	bool PrintErrors();

	// Getters:
	/* Get the Settings: */
	const SETTINGS& GetSettings();

	// Private functions:
private:

	// Member:
	// Public Member:
public:

	// System Components
	/* Errorhandler. */
	ErrorHandler errorHandler;

	// Subsystems:
	/* Physics System using Box2D */
	Physics physics;
	/* Input System using DirectInput */
	Input input;
	/* Sound System using DirectSound */
	Sound sound;
	/* Graphic System using DirectX 9.0c */
	Graphics graphics;

	// Private Member:
private:
	/* Settings*/
	const SETTINGS& settings;
};

