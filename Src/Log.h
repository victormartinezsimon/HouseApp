#pragma once
#include <string>

class Log
{

public:
	enum class  LOG_TYPE
	{
		LOG_NORMAL,
		LOG_ERROR,
		LOG_FATAL,
	};


public:

    void WriteLog(const std::string& log, LOG_TYPE type = LOG_TYPE::LOG_NORMAL);

private:
	std::string GetFormattedTime();
};