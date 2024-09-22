#include "Log.h"
#include <iostream>
#include <ctime>
#include <assert.h>

void  Log::WriteLog(const std::string& log, LOG_TYPE type)
{
    auto time = GetFormattedTime();
    std::string msg = "[" + time + "]: " + log + "\n";

    switch (type)
    {
    case Log::LOG_TYPE::NORMAL:
        std::cout << msg;
        break;
    case Log::LOG_TYPE::ERROR:
        std::cerr << msg;

#if _DEBUG
        assert(false && msg.c_str());
#endif

        break;
    case Log::LOG_TYPE::FATAL:
        std::cerr << msg;
        
        break;
    }

}

std::string Log::GetFormattedTime()
{
    time_t timestamp = time(NULL);
    struct tm datetime = *localtime(&timestamp);

    std::string toReturn =  "";
    toReturn += std::to_string(datetime.tm_hour) + ":" + std::to_string(datetime.tm_min) + ":" + std::to_string(datetime.tm_sec) + " ";
    toReturn += std::to_string(datetime.tm_mday) + "/" + std::to_string(datetime.tm_mon + 1) + "/" + std::to_string(datetime.tm_year + 1900);

    return toReturn;

}