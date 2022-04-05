//////////////////////////////////////////////////////////////////
// Orchid 2D Framework
// Version: Alpha 0.1
// File: Physics.cpp
// Purpose: - This Class is responsible for Physics calculations
//			- It uses Box2D for it's calculations
//////////////////////////////////////////////////////////////////

// Includes:
#include "Physics.h"

// Functions:
// Public Functions:
Physics::Physics()
	:
	errorHandler(ErrorHandler::GetInstance())
{
}
Physics::~Physics()
{
}

void Physics::Initialize()
{
	errorHandler->WriteLog("Initialiazing Physicsystem ...");

	errorHandler->WriteLog("Physicsystem Initialized.");

	return;
}
void Physics::Shutdown()
{
	// No Pointers or shutdownable subsystems, so not needed!
	errorHandler->WriteLog("Shutdown Physicsystem ...");


	errorHandler->WriteLog("Physicsystem Shutdown successful.");

	return;
}