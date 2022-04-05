//////////////////////////////////////////////////////////////////
// Orchid 2D Framework
// Version: Alpha 0.1
// File: Errorhandler.h
// Purpose: Errorhandler for reacting to errors and creating a Logfile
// Tasks:
//		- TODO:
//			- Hard Errors: NEED to abort
//			- Soft Errors: workaround or so
//			- Create Logfile
//////////////////////////////////////////////////////////////////

// Preprocessor Directives:
#pragma once

// Includes:
#include <windows.h>
#include <string>
#include <queue>
#include <cassert>

/* Class for Handling Errors. */
struct ErrorHandler final
{
	// Functions:
	// public Functions:
public:
	ErrorHandler(ErrorHandler* instance);
	~ErrorHandler();

	static ErrorHandler* GetInstance();

	// Write to Log (Debug: CMD or Release: File)
	void WriteLog(std::string LogString);

	// FatalError = Shutdown Engine
	void CreateFatalError(std::string ErrorString);
	// MinorError = Notification and Autofix if possible
	void CreateMinorError(std::string ErrorString);

	// Print all Errors; true = there was at least one fatal error!
	bool PrintErrors();

	// Member:
	// private Member:
private:
	// Own Instance
	static ErrorHandler *instance;

	std::queue<std::string> fatalerrorQueue;
	std::queue<std::string> minorerrorQueue;
};

