#pragma once
#include <string>
#include <map>
#include <vector>

#include "rapidjson/document.h"

class GeneralConfig
{

public:
    void Parse(const std::string& path);
    std::string GetValueString(const std::string& key) const;
    int GetValueInt(const std::string& key) const;
    bool GetValueBool(const std::string& key) const;

private:

    std::string GetValueAsString(rapidjson::Value& value)const;

    std::map< std::string, std::string> _params;
};