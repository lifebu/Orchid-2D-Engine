//////////////////////////////////////////////////////////////////
// Orchid 2D Framework
// Version: Alpha 0.1
// File: Layer.h
// Purpose: - A very generic Layer Class, that every Layertype
//			- derives from to get a generic view to layers.
//////////////////////////////////////////////////////////////////

// Preprocessor Directives:
#pragma once

// Includes
#include <string>
#include <vector>
#include <d3dx9.h>
#include "core/system/ErrorHandler.h"
#include "core/system/resources/ResourceManager.h"

/* Tileset Class. */
class Layer
{
	// Functions:
	// Public functions:
public:
	Layer();
	~Layer();


	bool IsInitialized() const;

	// Private functions:
private:

	// Member:
	// Public Member:
public:

	// Private Member:
protected:
	/* The Tilelayer is initialized. */
	bool isInitialized;

};