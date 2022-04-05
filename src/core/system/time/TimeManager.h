//////////////////////////////////////////////////////////////////
// Orchid 2D Framework
// Version: Alpha 0.1
// File: TimeManager.h
// Purpose: - This Class is responsible for managing time
//            you can add watches, and they will be set true etc.
//			- Also has functionality for FPS Calculation
//			 and Realtime based Movements
//////////////////////////////////////////////////////////////////

// Preprocessor Directives:
#pragma once

// Includes
#include <vector>
#include <cassert>
#include <windows.h>
#include "Watch.h"
#include "core/system/ErrorHandler.h"

class TimeManager final
{
	// Functions:
	// Public functions:
public:
	TimeManager();
	~TimeManager();

	/* Initialize the TimeManager. */
	void Initialize();
	/* Shutdown the TimeManager. */
	void Shutdown();

	// Get Instance
	static TimeManager* GetInstance();

	// Watch Functionality:
	/* Start all Watches (mostly at the beginning of the Game). */
	void StartWatches();
	/* Stop all Watches (mostly at the End of the Game). */
	void StopWatches();
	/* Start ONE Watch. */
	void StartWatch(std::string name);
	/* Stop ONE Watch. */
	void StopWatch(std::string name);

	/* Adds a new Watch. */
	void AddWatch(double frequency, std::string name);
	/* Delete a certain Watch. */
	void DeleteWatch(std::string name);
	/* Delete all Watch. */
	void ClearWatches();

	/* This will set the Watch.IsReached to true if the Watch has reached its frequency point.
	* If the Watch is currently set true, although it is not its time it's set to false! (was set in last frame!)*/
	void UpdateWatches();
	/* tells if the WatchtimeisReached. */
	bool IsWatchtimeReached(std::string name);

	// FPS Functionality:
	/* Update returns the Time difference for the frame.
	* Use this to Make Movements based in Realtime and not FPS. */
	const float Update();

	/* Start the Timer (at the Beginning of the Frame).*/
	void StartFPSTimer();
	/* Stop the Timer (at the End of the Frame). It calculates the Time Difference.*/
	void StopFPSTimer();
	/* Set the Refreshrate for the FPS Display.
	* specified in: [Refresh/Second].*/
	void SetFPSRefreshrate(float refreshRate);
	/* Only returns the real FPS if it shall be drawn (don't draw FPS every frame!).
	* specified in: [Refresh/Second].
	* if it shouldn't be drawn it returns -1.0f! */
	const float GetDrawFPS();
	
	// Private functions:
private:

	// Member:
	// Public Member:
public:

	// Private Member:
private:
	// Own Instance (Singleton!)
	static TimeManager* instance;

	// Watch Functionality:
	/* This DeltaTimes are the seperate DeltaTimes the TimeManager will be watching for.
	If one of this times is reached*/
	std::vector<WATCH> watches;

	// FPS Functionality:
	/* The Current FPS. */
	float currentCalculatedFPS;
	/* Is the Timer active? */
	bool isFPSTimerRunning;
	/* StartTime of the Counter. */
	long long fpsStartTime;
	/* EndTime of the Counter. */
	long long fpsEndTime;
	/* The Frequency of the Performance Timer. */
	long long performanceFrequency;
	/* Timedifference in Seconds*/
	double fpsDeltaTime;
	/* Refreshrate for the FPS Display. */
	float fpsRefreshrate;
	/* StartTime of the Counter. */
	long long startTimeFPSRefresh;
	/* EndTime of the Counter. */
	long long endTimeFPSRefresh;

	ErrorHandler* errorHandler;
};