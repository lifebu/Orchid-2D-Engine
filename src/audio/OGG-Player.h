//////////////////////////////////////////////////////////////////
// Orchid 2D Framework
// Version: Alpha 0.1
// File: OGG-Player.h
// Purpose: - This Class is a Player for OGG-Files
//////////////////////////////////////////////////////////////////

// Preprocessor Directives:
#pragma once

// Includes
#include "core/system/ErrorHandler.h"
#include <mmsystem.h>
#include <Windows.h>
#include <mmreg.h>
#include <dsound.h>
#include <DxErr.h>
#include <string>

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


// DEFINES
/* Length of the Buffer */
#define     BUFSIZE    65536*10

class OGG_Player final
{
	// Functions:
	// Public functions:
public:
	OGG_Player();
	~OGG_Player();

	// OGG-Functions:
	/* Load a OGG-File */
	void Load(const LPDIRECTSOUND8 dSound, const std::string filename);	
	/* Close a File */
	void Close();			
	/* Play the File */
	void Play(bool loop = false);		
	/* Stop Playing */
	void Stop();
	/* Update (use from time to time!) */
	void Update();		
	/* Is the File Playing? */
	bool IsPlaying();					

	// Private functions:
private:

	// Member:
	// Public Member:
public:

	// Private Member:
private:

	// OGG-Member:
	/* have we opened an ogg yet? */
	bool fileOpened;
	/* the buffer */
	LPDIRECTSOUNDBUFFER dSoundbuffer;  
	/* for the vorbisfile interface */
	OggVorbis_File vorbisFile;    
	/* which half of the buffer are/were */
	/* we playing? */
	int         lastSection,	
				curSection;				

	/* only one half of the buffer to play */
	bool        almostDone;	
	/* done playing */
	bool        done;
	/* loop? */
	bool        loop;	
	/* is it playin' dude? */
	bool		isPlaying;					
};