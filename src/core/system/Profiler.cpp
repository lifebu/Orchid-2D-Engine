//////////////////////////////////////////////////////////////////
// Orchid 2D Framework
// Version: Alpha 0.1
// File: Profiler.cpp
// Purpose: - This Class is responsible for profiling
//			- you can add variables that shall be watched over.
//////////////////////////////////////////////////////////////////

// Includes:
#include "Profiler.h"

// Functions:
// Public Functions://////////////////////////////////////////////////////////////////
Profiler::Profiler()
	:
	errorHandler(ErrorHandler::GetInstance())
{
}
Profiler::~Profiler()
{
}


void Profiler::Initialize()
{
	errorHandler->WriteLog("Initialiazing Profiler ...");

	errorHandler->WriteLog("Profiler Initialized.");

	return;
}
void Profiler::Shutdown()
{
	errorHandler->WriteLog("Shutdown Profiler ...");

	if (instance != nullptr)
	{
		delete instance;
		instance = nullptr;
	}

	errorHandler->WriteLog("Profiler Shutdown successful.");

	return;
}

Profiler* Profiler::GetInstance()
{
	if (instance == nullptr)
	{
		instance = new Profiler();
	}
	return instance;
}


Profiler* Profiler::instance = nullptr;