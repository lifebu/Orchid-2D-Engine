//////////////////////////////////////////////////////////////////
// Orchid 2D Framework
// Version: Alpha 0.1
// File: Camera.cpp
// Purpose: - This Class is a Base Camera
//			- Child Cameras are available (orthogonal & perspective)
//////////////////////////////////////////////////////////////////

// Preprocessor Directives:
#pragma once

// Includes
#include "Camera.h"

// Functions:
// Public functions:
Camera::Camera()
	:
	initialized(false)
{
	D3DXMatrixIdentity(&viewmatrix);
	D3DXMatrixIdentity(&projectionmatrix);
}
Camera::~Camera()
{

}

bool Camera::IsInitialized() const
{
	return initialized;
}


