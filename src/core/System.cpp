//////////////////////////////////////////////////////////////////
// Orchid 2D Framework
// Version: Alpha 0.1
// File: System.h
// Purpose: The Systemclass for the Framework
//		    Holding together all the Subsystems and important
//          System Components!
//////////////////////////////////////////////////////////////////

// Includes:
#include "System.h"

// Functions:
// public:
System::System(const SETTINGS& settings, const HWND hWnd, const HINSTANCE appInstance)
	:
	errorHandler(&errorHandler),
	settings(settings),
	graphics(settings, hWnd),
	input(appInstance, hWnd),
	sound(hWnd)
{
}
System::~System()
{
}

void System::FrameCycle(const Tilemap& tilemap)
{
	// Do Stuff and so on.
	
	// 1st: Update all Watches
	TimeManager::GetInstance()->UpdateWatches();

	// 2nd: Get Input
	/* Get Newest Info from the Input Devices. */
	input.UpdateInput();

	// 3rd: Calculate Movement
	
	// 4th: Draw Frame
	// 4.1.: BeginFrame:
	graphics.BeginFrame();

	// 4.2. Render Tilemap
	graphics.RenderTilemap(tilemap);

	// 4.3: End Frame:
	graphics.EndFrame();

	// 5th: Sound Update
	if (TimeManager::GetInstance()->IsWatchtimeReached("OGGFreq"))
	{
		sound.Update();
	}

	// 6th: Draw FPS:
	char buffer[40];
	float drawFPS = TimeManager::GetInstance()->GetDrawFPS();
	if (drawFPS != -1.0f)
	{
		sprintf(buffer, "FPS: %0.2f", drawFPS);
		graphics.DrawText(buffer, 0, 44, D3DCOLOR_XRGB(255, 255, 255));
	}

	return;
}

bool System::Initialize()
{
	// 2nd: Write to Log:
	errorHandler.WriteLog("Initializing Framework...");

	// 3rd: Initialize the SystemComponents
	errorHandler.WriteLog("");
	errorHandler.WriteLog("SystemComponents:");

	ResourceManager::GetInstance()->Initialize(graphics.GetDevice());
	TimeManager::GetInstance()->Initialize();
	Profiler::GetInstance()->Initialize();


	// 4th: Initialize the Subsystems:
	errorHandler.WriteLog("");
	errorHandler.WriteLog("Subsystems:");

	physics.Initialize();
	input.Initialize();
	sound.Initialize();
	graphics.Initialize();

	// 6th: Print all Errors, that occured during System::Initialize
	if (PrintErrors())
	{
		// No Fatal Errors!
		errorHandler.WriteLog("Framework Initialization successful.");
		errorHandler.WriteLog("");
	}
	else
	{
		errorHandler.WriteLog("Framework Initialization failed, fatal Errors detected!");
		return false;
	}

	// 7th: Add Basic Watches
	TimeManager::GetInstance()->AddWatch(3.0f, "OGGFreq");
	TimeManager::GetInstance()->AddWatch(0.12f, "AnimFreq");
	TimeManager::GetInstance()->StartWatches();

	return true;
}
void System::Shutdown()
{
	// 1st: Write to Log:
	errorHandler.WriteLog("");
	errorHandler.WriteLog("Shutdown Framework...");

	// 3rd: Shutdown the Subsystems:
	errorHandler.WriteLog("");
	errorHandler.WriteLog("Subsystems:");

	physics.Shutdown();
	input.Shutdown();
	sound.Shutdown();
	graphics.Shutdown();

	// 2nd: Shutdown the SystemComponents
	errorHandler.WriteLog("");
	errorHandler.WriteLog("SystemComponents:");

	Profiler::GetInstance()->Shutdown();
	TimeManager::GetInstance()->Shutdown();
	ResourceManager::GetInstance()->Shutdown();
	
	
	// 3th: Print all Errors, that occured during System::Shutdown
	if (PrintErrors())
	{
		// No Fatal Errors!
		errorHandler.WriteLog("Framework Shutdown successful.");
		errorHandler.WriteLog("");
	}
	else
	{
		errorHandler.WriteLog("Framework Shutdown failed, fatal Errors detected!");
	}

	return;
}

bool System::PrintErrors()
{
	if (errorHandler.PrintErrors())
	{
		// False = Fatal Errors detected.
		return false;
	}
	
	// True = No Fatal Errors
	return true;
}

const SETTINGS& System::GetSettings()
{
	return settings;
}