//////////////////////////////////////////////////////////////////
// Orchid 2D Framework
// Version: Alpha 0.1
// File: Physics.h
// Purpose: - This Class is responsible for Physics calculations
//			- It uses Box2D for it's calculations
//////////////////////////////////////////////////////////////////

// Preprocessor Directives:
#pragma once

// Includes
#include "core/system/ErrorHandler.h"

class Physics final
{
	// Functions:
	// Public functions:
public:
	Physics();
	~Physics();

	/* Initialize the TimeManager. */
	void Initialize();
	/* Shutdown the TimeManager. */
	void Shutdown();

	// Private functions:
private:

	// Member:
	// Public Member:
public:

	// Private Member:
private:
	ErrorHandler* errorHandler;

};