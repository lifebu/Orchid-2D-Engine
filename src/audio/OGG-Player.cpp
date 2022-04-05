//////////////////////////////////////////////////////////////////
// Orchid 2D Framework
// Version: Alpha 0.1
// File: OGG-Player.cpp
// Purpose: - This Class is a Player for OGG-Files
//////////////////////////////////////////////////////////////////
// Includes:
#include "OGG-Player.h"

// Functions:
// Public Functions:
OGG_Player::OGG_Player()
	:
	fileOpened(false),
	dSoundbuffer(nullptr),
	loop(false),
	done(false),
	almostDone(false),
	isPlaying(false)
{
}
OGG_Player::~OGG_Player()
{
	if (fileOpened)
	{
		Close();
	}
}

void OGG_Player::Load(LPDIRECTSOUND8 dSound, std::string filename)
{
	HRESULT result;
	int result2;
	ErrorHandler* errorHandler = ErrorHandler::GetInstance();

	if (dSound == nullptr)
	{
		// Need a valid Directsound Interface to work
		return;
	}

	// 1st: Is a file already opened? If so close it
	// hi
	if (fileOpened)	
	{
		Close();
	}

	// 2nd: Open a file
	FILE *file;
	file = fopen(filename.c_str(), "rb");
	if (!file)	// File couldn't be opened!
	{
		return;
	}

	// 3rd: Open that file as a OGG Vorbis File.
	result2 = ov_open(file, &vorbisFile, NULL, 0);	// Open the OGG File.

	// vorbis infro structure for the most important format info:
	vorbis_info *vorbisInfo = ov_info(&vorbisFile, -1);

	// 4th: Set the WaveFormat:
	WAVEFORMATEX waveFormat;
	ZeroMemory(&waveFormat, sizeof(WAVEFORMATEX));

	waveFormat.cbSize = sizeof(waveFormat);
	waveFormat.nChannels = vorbisInfo->channels;
	waveFormat.wBitsPerSample = 16;		// ogg vorbis is always 16 bit
	waveFormat.nSamplesPerSec = vorbisInfo->rate;
	waveFormat.nAvgBytesPerSec = waveFormat.nSamplesPerSec * waveFormat.nChannels * 2;
	waveFormat.nBlockAlign = 2 * waveFormat.nChannels;
	waveFormat.wFormatTag = 1;

	// 5th: Setup the Buffer
	DSBUFFERDESC bufferDesc;
	bufferDesc.dwSize = sizeof(bufferDesc);
	bufferDesc.dwFlags = DSBCAPS_CTRLVOLUME |			// We want to controll the Volume.
						 DSBCAPS_LOCDEFER |				// DSound decide if this Soundbuffer is in Hardware or Software memory.
						 DSBCAPS_STATIC;				// Static Soundbuffer (highly possible to be set in the Hardware Memory -> faster)
	bufferDesc.lpwfxFormat = &waveFormat;
	bufferDesc.dwReserved = 0;
	bufferDesc.dwBufferBytes = BUFSIZE * 2;

	result = dSound->CreateSoundBuffer(&bufferDesc, &dSoundbuffer, NULL);
	if (FAILED(result))
	{
		char buffer[256];
		sprintf(buffer, "Sound.Load(): Failed to Create the SoundBuffer: %s, %s", DXGetErrorStringA(result), DXGetErrorDescriptionA(result));
		errorHandler->CreateFatalError(buffer);
		return;
	}

	// 6th: Fill the Buffer
	DWORD pos = 0;
	int   sec = 0;
	int   ret = 1;
	DWORD size = BUFSIZE * 2;

	char *buf;

	// 7th: Lock the Soundbuffer
	result = dSoundbuffer->Lock(0, size, (LPVOID*)&buf, &size, NULL, NULL, DSBLOCK_ENTIREBUFFER);
	if (FAILED(result))
	{
		char buffer[256];
		sprintf(buffer, "Sound.Load(): Failed to Lock the SoundBuffer: %s, %s", DXGetErrorStringA(result), DXGetErrorDescriptionA(result));
		errorHandler->CreateFatalError(buffer);
		return;
	}

	// 8th: Now read in the bits
	while (ret && pos < size)
	{
		ret = ov_read(&vorbisFile, buf + pos, size - pos, 0, 2, 1, &sec);
		pos += ret;
	}

	// 9th: Unlock the Soundbuffer
	result = dSoundbuffer->Unlock(buf, size, NULL, NULL);
	if (FAILED(result))
	{
		char buffer[256];
		sprintf(buffer, "Sound.Load(): Failed to Unlock the SoundBuffer: %s, %s", DXGetErrorStringA(result), DXGetErrorDescriptionA(result));
		errorHandler->CreateFatalError(buffer);
		return;
	}

	// 10th: Set the first selection.
	curSection = lastSection = 0;

	fileOpened = true;
	return;
}
void OGG_Player::Close()
{
	HRESULT result;
	fileOpened = false;
	isPlaying = false;
	ErrorHandler* errorHandler = ErrorHandler::GetInstance();

	// 1st: Stop the Soundbuffer
	result = dSoundbuffer->Stop();
	if (FAILED(result))
	{
		char buffer[256];
		sprintf(buffer, "Sound.Close(): Failed to Stop the SoundBuffer: %s, %s", DXGetErrorStringA(result), DXGetErrorDescriptionA(result));
		errorHandler->CreateFatalError(buffer);

		ov_clear(&vorbisFile);

		if (dSoundbuffer != nullptr)
		{
			dSoundbuffer->Release();
			dSoundbuffer = nullptr;
		}

		return;
	}
	// 2nd: Clear the Vorbisfile and release the Soundbuffer
	ov_clear(&vorbisFile);

	if (dSoundbuffer != nullptr)
	{
		dSoundbuffer->Release();
		dSoundbuffer = nullptr;
	}
}
void OGG_Player::Play(bool loop)
{
	HRESULT result;
	ErrorHandler* errorHandler = ErrorHandler::GetInstance();

	if (!fileOpened)
	{
		return;
	}

	// 1st: play looping bevause we will fill the buffer;
	dSoundbuffer->SetVolume(-2000);
	result = dSoundbuffer->Play(0, 0, DSBPLAY_LOOPING);
	if (FAILED(result))
	{
		char buffer[256];
		sprintf(buffer, "Sound.Play(): Failed to Play the SoundBuffer: %s, %s", DXGetErrorStringA(result), DXGetErrorDescriptionA(result));
		errorHandler->CreateFatalError(buffer);
		return;
	}

	loop = loop;
	done = false;
	almostDone = false;
	isPlaying = true;
}
void OGG_Player::Stop()
{
	HRESULT result;
	ErrorHandler* errorHandler = ErrorHandler::GetInstance();

	if (!fileOpened)
	{
		return;
	}

	// 1st: Stop Playing
	result = dSoundbuffer->Stop();
	if (FAILED(result))
	{
		char buffer[256];
		sprintf(buffer, "Sound.Play(): Failed to Play the SoundBuffer: %s, %s", DXGetErrorStringA(result), DXGetErrorDescriptionA(result));
		errorHandler->CreateFatalError(buffer);
		return;
	}

	isPlaying = false;
}
void OGG_Player::Update()
{
	HRESULT result;
	DWORD pos;
	ErrorHandler* errorHandler = ErrorHandler::GetInstance();

	// 1st: Get the current position of the Buffer
	result = dSoundbuffer->GetCurrentPosition(&pos, NULL);
	if (FAILED(result))
	{
		char buffer[256];
		sprintf(buffer, "Sound.Update(): Failed to Update the SoundBuffer: %s, %s", DXGetErrorStringA(result), DXGetErrorDescriptionA(result));
		errorHandler->CreateFatalError(buffer);
		return;
	}

	// Is it still in the Buffer?
	curSection = pos < BUFSIZE ? 0 : 1;

	// 2nd: section changed?
	if (curSection != lastSection)
	{
		if (done && !loop)
		{
			Stop();
		}

		// 3rd: gotta use this trick 'cause otherwise there wont be played all the bits
		if (almostDone && !loop)
		{
			done = true;
		}

		DWORD size = BUFSIZE;
		char *buf;

		// 5th: fill the section we just left
		result = dSoundbuffer->Lock(lastSection * BUFSIZE, size, (LPVOID*)&buf, &size, NULL, NULL, 0);
		if (FAILED(result))
		{
			char buffer[256];
			sprintf(buffer, "Sound.Update(): Failed to Play the SoundBuffer: %s, %s", DXGetErrorStringA(result), DXGetErrorDescriptionA(result));
			errorHandler->CreateFatalError(buffer);
			return;
		}

		DWORD pos = 0;
		int sec = 0;
		int ret = 1;

		while (ret && pos < size)
		{
			ret = ov_read(&vorbisFile, buf + pos, size - pos, 0, 2, 1, &sec);
			pos += ret;
		}

		// 6th: reached the end?
		if (!ret && loop)
		{
			// we are looping so restart from the beginning
			// NOTE: sound with sizes smaller than BUFSIZE may be cut off

			ret = 1;
			ov_pcm_seek(&vorbisFile, 0);
			while (ret && pos < size)
			{
				ret = ov_read(&vorbisFile, buf + pos, size - pos, 0, 2, 1, &sec);
				pos += ret;
			}
		}
		else if (!ret && !(loop))
		{
			// 7th: not looping so fill the rest with 0
			while (pos < size)
			{
				*(buf + pos) = 0;
				pos++;
			}

			// 8th: and say that after the current section no other sections follows
			almostDone = true;
		}

		// 9th: Unlock Soundbuffer
		result = dSoundbuffer->Unlock(buf, size, NULL, NULL);
		if (FAILED(result))
		{
			char buffer[256];
			sprintf(buffer, "Sound.Update(): Failed to Play the SoundBuffer: %s, %s", DXGetErrorStringA(result), DXGetErrorDescriptionA(result));
			errorHandler->CreateFatalError(buffer);
			return;
		}

		lastSection = curSection;
	}
}
bool OGG_Player::IsPlaying()
{
	return isPlaying;
}