#include "GeneralConfig.h"

#include "Log.h"
#include <fstream>        // For read json file
#include <sstream>        // To conver json file to string

using namespace rapidjson;

GeneralConfig::GeneralConfig(Log* log) : _log(log) {}

std::string GeneralConfig::ReadFile(const std::string& path)
{
    std::ifstream file(path);
    if (!file.is_open())
    {
        _log->WriteLog("Error reading file: " + path, Log::LOG_TYPE::LOG_FATAL);
    }

    std::stringstream buffer;
    buffer << file.rdbuf();  // Leer el contenido del archivo en el buffer
    return buffer.str();     // Convertir el buffer a string
}

void GeneralConfig::Parse(const std::string& path)
{
    std::string jsonContent = ReadFile(path);

    // Crear el documento JSON
    rapidjson::Document doc;

    // Parsear el JSON
    if (doc.Parse(jsonContent.c_str()).HasParseError()) {
        _log->WriteLog("Error parsing file: " + path, Log::LOG_TYPE::LOG_FATAL);
        return;
    }

    for (auto& m : doc.GetObject())
    {
        std::string key = m.name.GetString();

        std::string value = SaveValueAsString(m.value);
        _params.insert({ key, value });
    }
}

std::string GeneralConfig::SaveValueAsString(rapidjson::Value& value)const
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

std::string GeneralConfig::GetValueString(const std::string& key, const std::string& defaultValue) const
{
    if (_params.find(key) == _params.end())
    {
        _log->WriteLog("key: " + key + " missing on GetValueString()", Log::LOG_TYPE::LOG_NORMAL);
        return defaultValue;
    }
    return _params.at(key);
}

int GeneralConfig::GetValueInt(const std::string& key, int defaultValue) const
{
    if (_params.find(key) == _params.end())
    {
        _log->WriteLog("key: " + key + " missing on GetValueInt()", Log::LOG_TYPE::LOG_NORMAL);
        return defaultValue;
    }

    auto value = _params.at(key);
    return atoi(value.c_str());


}
bool GeneralConfig::GetValueBool(const std::string& key, bool defaultValue) const
{
    if (_params.find(key) == _params.end())
    {
        _log->WriteLog("key: " + key + " missing on GetValueBool()", Log::LOG_TYPE::LOG_NORMAL);
        return defaultValue;
    }

    auto value = _params.at(key);
    
    return value == "1";
}