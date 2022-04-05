//////////////////////////////////////////////////////////////////
// Orchid 2D Framework
// Version: Alpha 0.1
// File: Profiler.h
// Purpose: - This Class is responsible for profiling
//			- you can add variables that shall be watched over.
//////////////////////////////////////////////////////////////////

// Preprocessor Directives:
#pragma once

// Includes
#include "ErrorHandler.h"

class Profiler final
{
	// Functions:
	// Public functions:
public:
	Profiler();
	~Profiler();

	/* Initialize the TimeManager. */
	void Initialize();
	/* Shutdown the TimeManager. */
	void Shutdown();

	// Get Instance
	static Profiler* GetInstance();

	// Private functions:
private:

	// Member:
	// Public Member:
public:

	// Private Member:
private:
	// Own Instance (Singleton!)
	static Profiler* instance;

	ErrorHandler* errorHandler;

};