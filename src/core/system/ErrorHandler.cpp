//////////////////////////////////////////////////////////////////
// Orchid 2D Framework
// Version: Alpha 0.1
// File: Errorhandler.cpp
// Purpose: Errorhandler for reacting to errors and creating a Logfile
//////////////////////////////////////////////////////////////////

// Includes:
#include "ErrorHandler.h"

// Functions:
// Public Functions:
ErrorHandler::ErrorHandler(ErrorHandler* instance)
{
#ifdef _DEBUG 
	// Only in Debug.
#else 
	// Release Mode
	// Delete an old Log file if it exists.
	// If there was none, one will be created.
	FILE* file = nullptr;
	file = fopen("log.txt", "w");
	fclose(file);

#endif
	ErrorHandler::instance = instance;
}
ErrorHandler::~ErrorHandler()
{
	if (instance)
	{
		// 1st: Write all Errors that has not been printed yet!
		instance->PrintErrors();

		// 2nd: Destroy the Instance
		instance = nullptr;
	}

	return;
}

ErrorHandler* ErrorHandler::GetInstance()
{

	assert(instance != nullptr);

	return instance;
}

void ErrorHandler::WriteLog(std::string LogString)
{
#ifdef _DEBUG 
	// Only in Debug.
	printf("%s\n", LogString.c_str());
#else
	// Release Mode
	// Write the Information into a txt-file
	FILE* file = nullptr;
	file = fopen("log.txt", "a");
	
	fprintf(file, "%s\n", LogString.c_str());


	fclose(file);
	
#endif
}

void ErrorHandler::CreateFatalError(std::string ErrorString)
{
	// 1st: Add the Fatal Error to the ErrorQueue
	fatalerrorQueue.push(ErrorString);
}
void ErrorHandler::CreateMinorError(std::string ErrorString)
{
	// 1st: Add the Minor Error to the ErrorQueue
	minorerrorQueue.push(ErrorString);
}

bool ErrorHandler::PrintErrors()
{
	// 1st: Where there any fatal errors?
	bool result = fatalerrorQueue.size() > 0;

	// 2nd: Process all Minor Errors.
	while (!minorerrorQueue.empty())
	{
		WriteLog("MinorError: " + minorerrorQueue.front());
		minorerrorQueue.pop();
	}

	// 3rd: Process all Fatal Errors.
	while (!fatalerrorQueue.empty())
	{
		WriteLog("FATALERROR: " + fatalerrorQueue.front());
		fatalerrorQueue.pop();
	}

	return result;
}

ErrorHandler* ErrorHandler::instance = nullptr;