#include "GeneralConfig.h"

#include "rapidjson/stringbuffer.h"
#include "rapidjson/filereadstream.h"

using namespace rapidjson;

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

std::string GeneralConfig::GetValueAsString(rapidjson::Value& value)
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

std::string GeneralConfig::GetValue(const std::string& key) const
{
    assert(_params.contains(key));
    return _params.at(key);
}