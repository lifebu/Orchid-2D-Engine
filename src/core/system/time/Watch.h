//////////////////////////////////////////////////////////////////
// Orchid 2D Framework
// Version: Alpha 0.1
// File: Watch.h
// Purpose: A Watch has a Name, the time passed before it
//			reacts (frequence) and the time it started
//////////////////////////////////////////////////////////////////

// Preprocessor Directives:
#pragma once

// Includes
#include <string>

struct WATCH
{
	double frequency;
	long long starttime;
	std::string name;
	bool isReached;
	bool isActive;
};