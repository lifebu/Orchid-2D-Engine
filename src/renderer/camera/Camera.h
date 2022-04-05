//////////////////////////////////////////////////////////////////
// Orchid 2D Framework
// Version: Alpha 0.1
// File: Camera.h
// Purpose: - This Class is a Base Camera
//			- Child Cameras are available (orthogonal & perspective)
//////////////////////////////////////////////////////////////////

// Preprocessor Directives:
#pragma once

// Includes
#include <d3dx9.h>
#include "core/system/ErrorHandler.h"

/* Base Camera Class. */
class Camera
{
	// Functions:
	// Public functions:
public:
	Camera();
	~Camera();

	// Getter:
	const D3DXMATRIX& GetViewMatrix() const;
	const D3DXMATRIX& GetProjectionMatrix() const;

	/* True = is initialized. */
	bool IsInitialized() const;

	// This function is not implemented, to prevent instantiating this base class!
	void Dummy();

	// Private functions:
private:

	// Member:
	// Public Member:
public:

	// Private Member:
protected:
	D3DXMATRIX viewmatrix;
	D3DXMATRIX projectionmatrix;
	bool initialized;

};