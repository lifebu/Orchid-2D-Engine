//////////////////////////////////////////////////////////////////
// Orchid 2D Framework
// Version: Alpha 0.1
// File: Game.cpp
// Purpose: The Programmed Game!
//////////////////////////////////////////////////////////////////

// Includes:
#include "Game.h"

// Functions:
// public:
Game::Game(const SETTINGS& settings, const HWND hWnd, const HINSTANCE appInstance)
	:
	system(settings, hWnd, appInstance),
	deltaTime(0.1666666f),
	settings(settings)
{
	errorHandler = ErrorHandler::GetInstance();
}
Game::~Game()
{
}

bool Game::FrameCycle()
{
	// 1st: Start the FPS Timer for this Frame
	TimeManager::GetInstance()->StartFPSTimer();

	

	for (auto& entry : animSprite)
	{
		entry.Update();
	}

	// 3rd: Start the System Framecycle
	system.FrameCycle(tilemap);

	// 4th: Handle the Input
	HandleInput();

	// 5th: Calculate the Deltatime
	TimeManager::GetInstance()->StopFPSTimer();
	deltaTime = TimeManager::GetInstance()->Update();

	// 6th: Print all Errors, and quit the Game if there were fatal errors:
	if (!system.PrintErrors())
	{
		// There were fatal Errors!
		// Return false for the OS-Handler, so it can initiate the Shutdown!
		return false;
	}

	return true;
}

bool Game::Initialize()
{
	// 1st: Initialize the Framework
	if (!system.Initialize())
	{
		// System hat Fatal Errors while Initializing, abort!
		return false;
	}

	// 2nd: Initialize Gamedata:
	// 2.1: initialize the Testsprites

	// 2.1.4: Animated Sprite
	Animated_Sprite temp4;
	//temp4.Initialize(system.graphics.GetDevice(), D3DXVECTOR3(-918.0f, -423.0f, 17.0f), "data/player.png", 6, 16, 16);
	temp4.Initialize(*system.graphics.GetDevice(), D3DXVECTOR3(-700.0f, -344.0f, 17.0f), "data/player.png", 6, 16, 16);
	animSprite.push_back(temp4);

	// 2.5: Initialize Sound:
	system.sound.AddPlayer("data/level1.ogg", "Level1");
	system.sound.Play(true, "Level1");

	// Test
	tilemap.Initialize("data/level1.tmx", *system.graphics.GetDevice());
	tilemap.AddObject(&animSprite.at(0));


	// 3rd: Print all Errors, that occured during Game::Initialize
	if (!system.PrintErrors())
	{
		return false;
	}

	return true;
}
void Game::Shutdown()
{
	// 1st: Delete all Sprites
	// 1.2. Animated Sprites
	for (auto& entry : animSprite)
	{
		entry.Shutdown();
	}
	animSprite.clear();

	tilemap.Shutdown();

	// 2nd: Shutdown System:
	system.Shutdown();

	// 3rd: Print all Errors, that occured during Game::Shutdown
	system.PrintErrors();

	return;
}

void Game::HandleInput()
{
	float gamepadMoveAmount = 0.6f;
	float keyboardMoveAmount = 0.6f;

	OrthogonalCamera& currentCam = system.graphics.GetActiveCamera();
	bool useGamepad = false;
	animSprite.at(0).SetLoop(false);

	if (system.input.KeyIsTipped(DIK_SPACE))
	{
		char buffer[60];
		sprintf(buffer, "X: %0.2f, Y: %0.2f", animSprite.at(0).GetPosition().x, animSprite.at(0).GetPosition().y);
		errorHandler->WriteLog(buffer);

		sprintf(buffer, "CamX: %0.2f, CamY: %0.2f", currentCam.GetPos().x, currentCam.GetPos().x);
		errorHandler->WriteLog(buffer);
	}


	if (useGamepad) // GamepadControlls
	{
		if (system.input.GamePadButtonIsTipped(1))
		{
			system.graphics.ToggleDebugCam();
		}
		if (system.input.GamePadButtonIsPressed(3))
		{
			gamepadMoveAmount *= 2.0f;
		}
	}
	else // Keyboard Controlls
	{
		if (system.input.KeyIsTipped(DIK_B))
		{
			system.graphics.ToggleDebugCam();
		}
		if (system.input.KeyIsPressed(DIK_LSHIFT))
		{
			keyboardMoveAmount *= 2.0f;
		}
	}

	// Gameplay Controls:
	if (!system.graphics.isDebugCamActive()) // Normal Cam Controlls
	{
		if (useGamepad)
		{
			currentCam.PanBy({ ((float)system.input.GetGamePadXAxis1() / 10.0f) * deltaTime * gamepadMoveAmount,
				-((float)system.input.GetGamePadYAxis1() / 10.0f) * deltaTime * gamepadMoveAmount });
			animSprite.at(0).TranslateBy({ ((float)system.input.GetGamePadXAxis1() / 10.0f) * deltaTime * gamepadMoveAmount,
				-((float)system.input.GetGamePadYAxis1() / 10.0f) * deltaTime * gamepadMoveAmount,
				0.0f });
			if (system.input.GamePadButtonIsPressed(4))	// LB
			{
				currentCam.ZoomBy(0.2f * deltaTime);
			}
			if (system.input.GamePadButtonIsPressed(5))	// RB
			{
				currentCam.ZoomBy(-0.2f * deltaTime);
			}

			if (((float)system.input.GetGamePadXAxis1() / 10.0f) * deltaTime != 0 ||
				((float)system.input.GetGamePadYAxis1() / 10.0f) * deltaTime != 0)
			{
				if (!animSprite.at(0).IsPlaying())
				{
					animSprite.at(0).Play(true);
				}
			}

			// flipping
			if (((float)system.input.GetGamePadXAxis1() / 10.0f) * deltaTime > 0.0f)
			{
				animSprite.at(0).SetFlip(false);
			}
			else if (((float)system.input.GetGamePadXAxis1() / 10.0f) * deltaTime < 0.0f)
			{
				animSprite.at(0).SetFlip(true);
			}

		}
		else
		{
			// Keyboard Controls:
			if (system.input.KeyIsPressed(DIK_W))
			{
				currentCam.PanBy({ 0.0f, 100.0f * deltaTime * keyboardMoveAmount });
				animSprite.at(0).TranslateBy({ 0.0f, 100.0f * deltaTime * keyboardMoveAmount, 0.0f });
				if (!animSprite.at(0).IsPlaying())
				{
					animSprite.at(0).Play(false);
				}
			}
			if (system.input.KeyIsPressed(DIK_S))
			{
				currentCam.PanBy({ 0.0f, -100.0f * deltaTime * keyboardMoveAmount });
				animSprite.at(0).TranslateBy({ 0.0f, -100.0f * deltaTime * keyboardMoveAmount, 0.0f });
				if (!animSprite.at(0).IsPlaying())
				{
					animSprite.at(0).Play(false);
				}
			}
			if (system.input.KeyIsPressed(DIK_A))
			{
				currentCam.PanBy({ -100.0f * deltaTime * keyboardMoveAmount, 0.0f });
				animSprite.at(0).TranslateBy({ -100.0f * deltaTime * keyboardMoveAmount, 0.0f, 0.0f });
				animSprite.at(0).SetFlip(true);
				if (!animSprite.at(0).IsPlaying())
				{
					animSprite.at(0).Play(false);
				}
			}
			if (system.input.KeyIsPressed(DIK_D))
			{
				currentCam.PanBy({ 100.0f * deltaTime * keyboardMoveAmount, 0.0f });
				animSprite.at(0).TranslateBy({ 100.0f * deltaTime * keyboardMoveAmount, 0.0f, 0.0f });
				animSprite.at(0).SetFlip(false);
				if (!animSprite.at(0).IsPlaying())
				{
					animSprite.at(0).Play(false);
				}
			}

			if (system.input.KeyIsPressed(DIK_NUMPADPLUS))
			{
				currentCam.ZoomBy(-0.2f * deltaTime);
			}
			if (system.input.KeyIsPressed(DIK_NUMPADMINUS))
			{
				currentCam.ZoomBy(0.2f * deltaTime);
			}
		}
	}
	else // Debug Cam Controlls:
	{
		OrthogonalCamera& debugCam = system.graphics.GetDebugCamera();
		if (useGamepad)
		{

			debugCam.PanBy({ ((float)system.input.GetGamePadXAxis1() / 10.0f) * deltaTime * gamepadMoveAmount,
				-((float)system.input.GetGamePadYAxis1() / 10.0f) * deltaTime * gamepadMoveAmount });

			if (system.input.GamePadButtonIsPressed(4))	// LB
			{
				debugCam.ZoomBy(0.2f * deltaTime);
			}
			if (system.input.GamePadButtonIsPressed(5))	// RB
			{
				debugCam.ZoomBy(-0.2f * deltaTime);
			}
		}
		else
		{
			// Keyboard Controls:
			if (system.input.KeyIsPressed(DIK_W))
			{
				debugCam.PanBy({ 0.0f, 100.0f * deltaTime * keyboardMoveAmount });
			}
			if (system.input.KeyIsPressed(DIK_S))
			{
				debugCam.PanBy({ 0.0f, -100.0f * deltaTime * keyboardMoveAmount });
			}
			if (system.input.KeyIsPressed(DIK_A))
			{
				debugCam.PanBy({ -100.0f * deltaTime * keyboardMoveAmount, 0.0f });
			}
			if (system.input.KeyIsPressed(DIK_D))
			{
				debugCam.PanBy({ 100.0f * deltaTime * keyboardMoveAmount, 0.0f });
			}

			if (system.input.KeyIsPressed(DIK_NUMPADPLUS))
			{
				debugCam.ZoomBy(-0.2f * deltaTime);
			}
			if (system.input.KeyIsPressed(DIK_NUMPADMINUS))
			{
				debugCam.ZoomBy(0.2f * deltaTime);
			}
		}
	}

	// Check Collision!
	float FishMinX = animSprite.at(0).GetPosition().x - (animSprite.at(0).GetWidth() / 2.0f);
	float FishMaxX = animSprite.at(0).GetPosition().x + (animSprite.at(0).GetWidth() / 2.0f);
	float FishMinY = animSprite.at(0).GetPosition().y - (animSprite.at(0).GetHeight() / 2.0f);
	float FishMaxY = animSprite.at(0).GetPosition().y + (animSprite.at(0).GetHeight() / 2.0f);
	Tilelayer* layer = nullptr;
	
	//// Get the right Background layer.
	//if (tilemap.IsInitialized())
	//{
	//	for (auto entry : tilemap.GetBackgroundTileLayers())
	//	{
	//		if (entry.getName() == "Background Tiles_1")
	//		{
	//			// This is the background layer used for collision!
	//			if (entry.IsInitialized())
	//			{
	//				layer = &entry;
	//			}
	//		}
	//	}

	//	// Iterate through all tiles.
	//	if (layer != nullptr)
	//	{
	//		for (auto& entry : layer->getTiles())
	//		{
	//			float TileMinX = entry.GetPosition().x - (tilemap.GetTileWidth() / 2.0f);
	//			float TileMaxX = entry.GetPosition().x + (tilemap.GetTileWidth() / 2.0f);
	//			float TileMinY = entry.GetPosition().y - (tilemap.GetTileHeight() / 2.0f);
	//			float TileMaxY = entry.GetPosition().y + (tilemap.GetTileHeight() / 2.0f);


	//		}
	//	}
	//}
}
