#include "GeneralConfig.h"

#include "Log.h"
#include "rapidjson/stringbuffer.h"
#include "rapidjson/filereadstream.h"

using namespace rapidjson;

GeneralConfig::GeneralConfig(Log* log) : _log(log) {}

void GeneralConfig::Parse(const std::string& path)
{
    FILE* fp = fopen(path.c_str(), "r"); // non-Windows use "r"

    char readBuffer[65536];
    FileReadStream is(fp, readBuffer, sizeof(readBuffer));

    Document doc;
    doc.ParseStream(is);

    for (auto& m : doc.GetObject())
    {
        std::string key = m.name.GetString();

        std::string value = GetValueAsString(m.value);
        _params.insert({ key, value });
    }
}

std::string GeneralConfig::GetValueAsString(rapidjson::Value& value)const
{
    if (value.IsBool())
    {
        return std::to_string(value.GetBool());
    }

    if (value.IsInt())
    {
        return std::to_string(value.GetInt());
    }

    if (value.IsFloat())
    {
        return std::to_string(value.GetFloat());
    }

    return value.GetString();
}

std::string GeneralConfig::GetValueString(const std::string& key) const
{
    if (_params.find(key) == _params.end())
    {
        _log->WriteLog("key: " + key + " missing on GetValueString()", Log::LOG_TYPE::LOG_ERROR);
    }
    return _params.at(key);
}

int GeneralConfig::GetValueInt(const std::string& key) const
{
    if (_params.find(key) == _params.end())
    {
        _log->WriteLog("key: " + key + " missing on GetValueInt()", Log::LOG_TYPE::LOG_ERROR);
    }

    auto value = _params.at(key);
    return atoi(value.c_str());


}
bool GeneralConfig::GetValueBool(const std::string& key) const
{
    if (_params.find(key) == _params.end())
    {
        _log->WriteLog("key: " + key + " missing on GetValueBool()", Log::LOG_TYPE::LOG_ERROR);
    }

    auto value = _params.at(key);
    
    return value == "1";
}