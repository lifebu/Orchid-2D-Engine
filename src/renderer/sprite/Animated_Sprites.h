//////////////////////////////////////////////////////////////////
// Orchid 2D Framework
// Version: Alpha 0.1
// File: Animated_Sprites.h
// Purpose: - This Class is used to render 2D Animated Sprites with Hardware Acceleration.
//			- It's composed of 2 triangles, which are rendered.
//////////////////////////////////////////////////////////////////

// Preprocessor Directives:
#pragma once

// Includes
#include <d3dx9.h>
#include <DxErr.h>
#include <string>
#include "Static_Sprite.h"
#include "core/system/ErrorHandler.h"
#include "renderer/GraphicDefinitions.h"
#include "core/system/time/TimeManager.h"

class Animated_Sprite : public Static_Sprite
{
	// Functions:
	// Public functions:
public:
	Animated_Sprite();
	~Animated_Sprite();

	/* Initialize the Graphic. */
	void Initialize(const LPDIRECT3DDEVICE9 d3dDevice, const D3DXVECTOR3 position,
		const std::string textureFile, const int numFrames, const int frameWidth, 
		const int frameHeight);
	///* Shutdown the Graphic. */
	//void Shutdown();

	//bool IsInitialized() const;
	//void RotateBy(D3DXVECTOR3 amount);
	//void ScaleBy(float amount);
	//void TranslateBy(D3DXVECTOR3 amount);

	// Animation functions:
	/* Is the Animation Playing? */
	bool IsPlaying();
	/* Play the Animation (resets if already playing). */
	void Play(bool loop = true);
	/* Need to call this every frame, to check if the Timestep is reached. */
	void Update();
	/* Pause the Animation. */
	void Pause();
	/* Stop the Animation. */
	void Stop();


	// Getters:
	const bool& GetLoop() const;
	const bool& GetFlip() const;

	// Setters
	void SetLoop(bool newValue);
	void SetFlip(bool newValue);

	// Private functions:
private:

	// Member:
	// Public Member:
public:

	// Private Member:
private:
	/* Is the Sprite flipped? */
	bool isFlipped;

	// Animation Data:
	/* Is the animation currently playing? */
	bool isPlaying;
	/* Shall the Animation loop?*/
	bool loop;
	/* Of how many frames does the animation is made of? */
	int numFrames;
	/* Width of one Frame (in Pixel). */
	int frameWidth;
	/* Height of one Frame (in Pixel). */
	int frameHeight;
	/* How many frames per Row. */
	int numFramesPerRow;
	/* Current Frame. */
	int currentFrame;
};

