#pragma once
#include <stdarg.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include <iostream>
#include <fstream>
#include <mutex>

using namespace std;

enum LogLevel {
	LogLevel_VERBOSE = 0,
	LogLevel_INFO = 1,
	LogLevel_WARNING = 2,
	LogLevel_ERROR = 3
};

static const char* LOG_LEVELS[] =
{
	"DEBUG | ",
	"INFO  | ",
	"WARN  | ",
	"ERROR | "
};

class _Logger
{
public:
	_Logger();
	~_Logger();
	void Log(LogLevel level, const char* msg, ...);
	void SetMinLevelStdout(LogLevel level);
	void SetMinLevelFile(LogLevel level);
	void SetFileName(const string &fileName);

private:
	void LogToFile(const string &s);
	LogLevel minLevelStdout = LogLevel_VERBOSE;
	LogLevel minLevelFile = LogLevel_WARNING;
	mutex mtx;
	string logFile = "C:\\Users\\Jan\\Desktop\\log.txt";
};

static _Logger Logger;