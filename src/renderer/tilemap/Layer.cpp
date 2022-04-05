//////////////////////////////////////////////////////////////////
// Orchid 2D Framework
// Version: Alpha 0.1
// File: Layer.cpp
// Purpose: - A very generic Layer Class, that every Layertype
//			- derives from to get a generic view to layers.
//////////////////////////////////////////////////////////////////

// Includes
#include "Layer.h"

// Functions:
// Public functions:
Layer::Layer()
	:
	isInitialized(false)
{
}
Layer::~Layer()
{
}

bool Layer::IsInitialized() const
{
	return isInitialized;
}