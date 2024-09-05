#pragma once
#include <string>
#include <map>
#include <vector>

#include "rapidjson/document.h"

class GeneralConfig
{

public:
    void Parse(const std::string& path);
    std::string GetValue(const std::string& key) const;

private:

    std::string GetValueAsString(rapidjson::Value& value);

    std::map< std::string, std::string> _params;
};