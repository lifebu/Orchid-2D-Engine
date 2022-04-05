//////////////////////////////////////////////////////////////////
// Orchid 2D Framework
// Version: Alpha 0.1
// File: Sound.h
// Purpose: - This Class is responsible for playing sounds and music.
//			- It uses DirectSound.
//////////////////////////////////////////////////////////////////

// Preprocessor Directives:
#pragma once

// Includes
#include "core/system/ErrorHandler.h"
#include "audio/OGG-Player.h"
#include <mmsystem.h>
#include <Windows.h>
#include <mmreg.h>
#include <dsound.h>
#include <DxErr.h>
#include <vector>

// OGG-File Vorbis Encoder
#include <vorbis\codec.h>
#include <vorbis\vorbisfile.h>

// Library dependencies
#pragma comment(lib, "DSound.lib")
#pragma comment(lib, "DXGUID.lib")
#pragma comment(lib, "DxErr.lib")
#pragma comment(lib, "libvorbisfile_static.lib")
#pragma comment(lib, "libvorbis_static.lib")
#pragma comment(lib, "libogg_static.lib")

// OGG Player Save structure
struct OGG_Save
{
	OGG_Player* player;
	std::string name;
};


// DEFINES!
#define     BUFSIZE    65536*10         // buffer length

class Sound final
{
	// Functions:
	// Public functions:
public:
	Sound(const HWND hWnd);
	~Sound();

	/* Initialize the Sound. */
	void Initialize();
	/* Shutdown the Sound. */
	void Shutdown();

	/* This needs too be called in an irregular basis (So Sounds work, proberly (every 1.5 Seconds)).*/
	void Update();

	// Player functions:
	/* Add a new Player */
	void AddPlayer(std::string filename, std::string Playername);
	/* Close a Player. */
	void Close(std::string Playername);
	/* Play a Player*/
	void Play(bool loop, std::string Playername);
	/* Stop a Player. */
	void Stop(std::string Playername);
	/* Is this Player playing?*/
	bool IsPlaying(std::string Playername);

	// Private functions:
private:

	// Member:
	// Public Member:
public:

	// Private Member:
private:
	const HWND hWnd;

	/* The Main DirectSound Interface. */
	LPDIRECTSOUND8 dSound;

	// Container for all OGG-Player
	std::vector<OGG_Save> players;

	ErrorHandler* errorHandler;

};

