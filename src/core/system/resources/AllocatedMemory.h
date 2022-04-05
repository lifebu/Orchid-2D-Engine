//////////////////////////////////////////////////////////////////
// Orchid 2D Framework
// Version: Alpha 0.1
// File: AllocatedMemory.h
// Purpose: Struct of where something was allocated
//////////////////////////////////////////////////////////////////

// Preprocessor Directives:
#pragma once

// Includes:
#include <string>

struct AllocatedMemory
{
	void* memory;
	int size;
	std::string file;
	int line;
};