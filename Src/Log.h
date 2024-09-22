#pragma once
#include <string>

class Log
{

public:
	enum class  LOG_TYPE
	{
		LOG_LOW,
		LOG_NORMAL,
		LOG_ERROR,
		LOG_FATAL,
	};


public:

    void WriteLog(const std::string& log, LOG_TYPE type = LOG_TYPE::LOG_LOW);

private:
	std::string GetFormattedTime();
};