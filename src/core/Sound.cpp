//////////////////////////////////////////////////////////////////
// Orchid 2D Framework
// Version: Alpha 0.1
// File: Sound.cpp
// Purpose: - This Class is responsible for playing sounds and music.
//			- It uses DirectSound.
//////////////////////////////////////////////////////////////////

// Includes:
#include "Sound.h"

// Functions:
// Public Functions:
Sound::Sound(const HWND hWnd)
	:
	dSound(nullptr),
	hWnd(hWnd),
	errorHandler(ErrorHandler::GetInstance())
{
}
Sound::~Sound()
{

}

void Sound::Initialize()
{
	errorHandler->WriteLog("Initialiazing Soundsystem ...");

	HRESULT result;

	// 1st: Create the DirectSound8 Main Interface.
	result = DirectSoundCreate8(NULL,					// Use the Default Playback Device
							    &dSound,				// The Interface to be created.
								NULL);					// Unused (Always NULL).
	if (FAILED(result))
	{
		char buffer[256];
		sprintf(buffer, "Sound.Initialize(): Failed to Create the DSound Interface: %s, %s", DXGetErrorStringA(result), DXGetErrorDescriptionA(result));
		errorHandler->CreateFatalError(buffer);
		return;
	}

	// 2nd: Setup the Cooperative Level.
	result = dSound->SetCooperativeLevel(hWnd,				// Windowhandle
											DSSCL_PRIORITY);	// The Application is able to determine the main Soundbuffer.
	if (FAILED(result))
	{
		char buffer[256];
		sprintf(buffer, "Sound.Initialize(): Failed to Set the DSound Cooperative Level: %s, %s", DXGetErrorStringA(result), DXGetErrorDescriptionA(result));
		errorHandler->CreateFatalError(buffer);
		return;
	}

	errorHandler->WriteLog("Soundsystem Initialized.");


	return;
}
void Sound::Shutdown()
{
	// No Pointers or shutdownable subsystems, so not needed!
	errorHandler->WriteLog("Shutdown Soundsystem ...");

	for (auto& entry : players)
	{
		entry.player->Close();
		delete entry.player;
	}
	players.clear();

	if (dSound != nullptr)
	{
		dSound->Release();
		dSound = nullptr;
	}
	
	errorHandler->WriteLog("Soundsystem Shutdown successful.");

	return;
}

void Sound::Update()
{
	// Update every OGGPlayer loaded.
	for (auto& entry : players)
	{
		entry.player->Update();
	}
}

// Player functions:
void Sound::AddPlayer(std::string filename, std::string Playername)
{
	OGG_Player* newPlayer;
	newPlayer = new OGG_Player;
	newPlayer->Load(dSound, filename);
	OGG_Save newSave;
	newSave.player = newPlayer;
	newSave.name = Playername;

	players.push_back(newSave);
}
void Sound::Close(std::string Playername)
{
	for (auto& entry : players)
	{
		if (entry.name == Playername)
		{	
			entry.player->Close();
		}
	}
}
void Sound::Play(bool loop, std::string Playername)
{
	for (auto& entry : players)
	{
		if (entry.name == Playername)
		{
			entry.player->Play(loop);
		}
	}
}
void Sound::Stop(std::string Playername)
{
	for (auto& entry : players)
	{
		if (entry.name == Playername)
		{
			entry.player->Stop();
		}
	}
}
bool Sound::IsPlaying(std::string Playername)
{
	for (auto& entry : players)
	{
		if (entry.name == Playername)
		{
			return entry.player->IsPlaying();
		}
	}

	return false;
}