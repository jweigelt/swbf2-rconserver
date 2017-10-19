#include "Logger.h"

_Logger::_Logger()
{
}

_Logger::~_Logger()
{
}

void _Logger::Log(LogLevel level, const char* msg, ...) {

	if (minLevelStdout <= level) {
		lock_guard <mutex> lg(mtx);
		{
			printf(LOG_LEVELS[level]);
			va_list args;
			va_start(args, msg);
			vprintf(msg, args);
			va_end(args);
			printf("\n");
		}
	}

	if (minLevelFile <= level) {
		//TODO: fix me
		LogToFile(msg);
	}
}

void _Logger::SetMinLevelStdout(LogLevel level)
{
	minLevelStdout = level;
}

void _Logger::SetMinLevelFile(LogLevel level)
{
	minLevelFile = level;
}

void _Logger::SetFileName(const string & fileName)
{
	logFile = fileName;
}

void _Logger::LogToFile(const string & s)
{
	ofstream f;
	f.open(logFile, f.app);
	f << s.c_str() << "\n";
	f.close();
}