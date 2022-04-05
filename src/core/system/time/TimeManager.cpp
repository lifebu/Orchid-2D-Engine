//////////////////////////////////////////////////////////////////
// Orchid 2D Framework
// Version: Alpha 0.1
// File: TimeManager.cpp
// Purpose: - This Class is responsible for managing time
//            you can add watches, and they will be set true etc.
//			- Also has functionality for FPS Calculation
//			 and Realtime based Movements
//////////////////////////////////////////////////////////////////

// Includes:
#include "TimeManager.h"

// Functions:
// Public Functions:
TimeManager::TimeManager()
	:
	errorHandler(ErrorHandler::GetInstance())
{
}
TimeManager::~TimeManager()
{
}


void TimeManager::Initialize()
{
	errorHandler->WriteLog("Initialiazing TimeManager ...");
	
	watches.clear();
	currentCalculatedFPS = 60.0f;
	isFPSTimerRunning = false;
	fpsStartTime = 0;
	fpsEndTime = 0;
	fpsDeltaTime = 0.1666666f;
	fpsRefreshrate = 1.0f;

	BOOL result;

	// 1st: Get the Frequency of the Performancetimer (how many counts per seconds)
	result = QueryPerformanceFrequency((LARGE_INTEGER*)&performanceFrequency);
	if (result == false)
	{
		char buffer[256];
		sprintf(buffer, "TimeManager.Initialize(): Failed to get the PerformanceFrequency: %i", (int)GetLastError() );
		errorHandler->CreateFatalError(buffer);
		return;
	}


	// 2nd: Get the first StartTime for the FPS refresher
	result = QueryPerformanceCounter((LARGE_INTEGER*)&startTimeFPSRefresh);
	if (result == false)
	{
		char buffer[256];
		sprintf(buffer, "TimeManager.Initialize(): Failed to query the PerformanceCounter: %i", (int)GetLastError());
		errorHandler->CreateFatalError(buffer);
		return;
	}

	errorHandler->WriteLog("TimeManager Initialized.");

	return;
}
void TimeManager::Shutdown()
{
	errorHandler->WriteLog("Shutdown TimeManager ...");

	if (instance != nullptr)
	{
		delete instance;
		instance = nullptr;
	}

	errorHandler->WriteLog("TimeManager Shutdown successful.");

	return;
}

TimeManager* TimeManager::GetInstance()
{
	if (instance == nullptr)
	{
		instance = new TimeManager();
	}
	return instance;
}

// Watch Functionality:
void TimeManager::StartWatches()
{
	BOOL result;

	// 1st: Set the Starttime of all Watches
	for (auto& entry : watches)
	{
		result = QueryPerformanceCounter((LARGE_INTEGER*)&entry.starttime);
		if (result == false)
		{
			char buffer[256];
			sprintf(buffer, "TimeManager.Initialize(): Failed to query the PerformanceCounter: %i", (int)GetLastError());
			errorHandler->CreateFatalError(buffer);
			return;
		}

		// 2nd: Set the Watches to active:
		entry.isActive = true;
	}
}
void TimeManager::StopWatches()
{
	for (auto& entry : watches)
	{
		// 2nd: Set all Watches to inactive:
		entry.isActive = false;
	}
}
void TimeManager::StartWatch(std::string name)
{
	HRESULT result;
	for (auto& entry : watches)
	{
		if (entry.name == name)
		{
			// This is the Watch!

			// 1st: Set the StartTime.
			result = QueryPerformanceCounter((LARGE_INTEGER*)&entry.starttime);
			if (result == false)
			{
				char buffer[256];
				sprintf(buffer, "TimeManager.Initialize(): Failed to query the PerformanceCounter: %i", (int)GetLastError());
				errorHandler->CreateFatalError(buffer);
				return;
			}

			// 2nd: Make this watch active!
			entry.isActive = true;
		}
	}
}
void TimeManager::StopWatch(std::string name)
{
	for (auto& entry : watches)
	{
		if (entry.name == name)
		{
			// This is the Watch!
			// 1st: Make this watch inactive!
			entry.isActive = false;
		}
	}
}

void TimeManager::AddWatch(double frequency, std::string name)
{
	// 1st: Create new Watch
	WATCH Temp;
	Temp.frequency = frequency;
	Temp.isReached = false;
	Temp.name = name;
	Temp.starttime = 0;
	Temp.isActive = false;

	// 2nd: Add it to the List
	watches.push_back(Temp);
}
void TimeManager::DeleteWatch(std::string name)
{
	auto iterator = watches.begin();
	while (iterator != watches.end())
	{
		// 1st: Is this entry the wanted watch?
		if (iterator->name == name)
		{
			// 2nd: erase it!
			watches.erase(iterator);
		}
	}
}
void TimeManager::ClearWatches()
{
	watches.clear();
}

void TimeManager::UpdateWatches()
{
	BOOL result;
		// Variables
		long long currentTime;
		double currentDeltaTime;

		// 1st: get currentTime
		result = QueryPerformanceCounter((LARGE_INTEGER*)&currentTime);
		if (result == false)
		{
			char buffer[256];
			sprintf(buffer, "TimeManager.Initialize(): Failed to query the PerformanceCounter: %i", (int)GetLastError());
			errorHandler->CreateFatalError(buffer);
			return;
		}

		// 2nd: get the Watch!
		for (auto& entry : watches)
		{
			// Only update active watches!
			if (entry.isActive)
			{
				// 3rd: Set the "Is Watch reached" of the Watch to false (it was set true the last frame, but isn't true this frame (logically!).
				entry.isReached = false;

				// 4th: Get the Current Delta Time between Current Time and the Start of the Watch!
				currentDeltaTime = (((double)(currentTime - entry.starttime)) / ((double)performanceFrequency));

				// 5th: Compare this to the WatchDelta
				if (currentDeltaTime >= entry.frequency)
				{
					// Get a new Starttime for the Watch!
					result = QueryPerformanceCounter((LARGE_INTEGER*)&entry.starttime);
					if (result == false)
					{
						char buffer[256];
						sprintf(buffer, "TimeManager.Initialize(): Failed to query the PerformanceCounter: %i", (int)GetLastError());
						errorHandler->CreateFatalError(buffer);
						return;
					}
					entry.isReached = true;
					return;
				}
			}
		}


	return;
}
bool TimeManager::IsWatchtimeReached(std::string name)
{
	for (auto& entry : watches)
	{
		// 1st: Is this the Watch wanted?
		if (entry.name == name)
		{
			if (entry.isReached == true)
			{
				return true;
			}
			else
			{
				return false;
			}
		}
	}

	char buffer[256];
	sprintf(buffer, "TimeManager.isWatchtimeReached: Watch of the Name: %s does not exist!", name.c_str());
	errorHandler->CreateMinorError(buffer);
	return false;
}

// FPS Functionality:
const float TimeManager::Update()
{
	return (float)fpsDeltaTime;
}

void TimeManager::StartFPSTimer()
{
	BOOL result;
	if (isFPSTimerRunning == false)
	{
		isFPSTimerRunning = true;
		// 1st: Get the Starttime for the FPS Calculation
		result = QueryPerformanceCounter((LARGE_INTEGER*)&fpsStartTime);
		if (result == false)
		{
			char buffer[256];
			sprintf(buffer, "TimeManager.Initialize(): Failed to query the PerformanceCounter: %i", (int)GetLastError());
			errorHandler->CreateFatalError(buffer);
			return;
		}
	}
}
void TimeManager::StopFPSTimer()
{
	BOOL result;
	if (isFPSTimerRunning == true)
	{
		isFPSTimerRunning = false;
		// 1st: Get the Endtime for the FPS Calculation
		result = QueryPerformanceCounter((LARGE_INTEGER*)&fpsEndTime);
		if (result == false)
		{
			char buffer[256];
			sprintf(buffer, "TimeManager.Initialize(): Failed to query the PerformanceCounter: %i", (int)GetLastError());
			errorHandler->CreateFatalError(buffer);
			return;
		}
		// 2nd: Calculate the Difference
		fpsDeltaTime = (((double)(fpsEndTime - fpsStartTime)) / ((double)performanceFrequency));
	}
}
void TimeManager::SetFPSRefreshrate(float refreshRate)
{
	fpsRefreshrate = refreshRate;
}
const float TimeManager::GetDrawFPS()
{
	BOOL result;

	// 1st: Get the Endtime for the Refresh
	result = QueryPerformanceCounter((LARGE_INTEGER*)&endTimeFPSRefresh);
	if (result == false)
	{
		char buffer[256];
		sprintf(buffer, "TimeManager.Initialize(): Failed to query the PerformanceCounter: %i", (int)GetLastError());
		errorHandler->CreateFatalError(buffer);
		return -1.0f;
	}

	if ((((double)(endTimeFPSRefresh - startTimeFPSRefresh)) / ((double)performanceFrequency)) >= fpsRefreshrate)
	{
		// 2nd: If we have reached the time, cacluate the current FPS, set the new Starttime and return the FPS.
		currentCalculatedFPS = 1.0f / (float)fpsDeltaTime;
		startTimeFPSRefresh = endTimeFPSRefresh;
		return currentCalculatedFPS;
	}

	return currentCalculatedFPS;
}

TimeManager* TimeManager::instance = nullptr;