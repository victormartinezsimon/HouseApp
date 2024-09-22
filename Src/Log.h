#pragma once
#include <string>

class Log
{

public:
	enum class  LOG_TYPE
	{
		NORMAL,
		ERROR,
		FATAL
	};


public:

    void WriteLog(const std::string& log, LOG_TYPE type = LOG_TYPE::NORMAL);

private:
	std::string GetFormattedTime();
};