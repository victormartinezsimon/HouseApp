#pragma once
#include <string>
#include <map>
#include <vector>

#include "rapidjson/document.h"

class Log;

class GeneralConfig
{

public:
    GeneralConfig(Log*);
    void Parse(const std::string& path);
    std::string GetValueString(const std::string& key, const std::string& defaultValue = "") const;
    int GetValueInt(const std::string& key, int defaultValue = 0) const;
    bool GetValueBool(const std::string& key, bool defaultValue = false) const;

private:

    std::string SaveValueAsString(rapidjson::Value& value)const;
    std::string ReadFile(const std::string& path);

    std::map< std::string, std::string> _params;

    Log* _log;
};