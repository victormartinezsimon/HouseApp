#pragma once
#include <string>
#include <map>
#include <vector>
#include <functional>

class Log;

#include "rapidjson/document.h"
#include "rapidjson/stringbuffer.h"
#include "rapidjson/filereadstream.h"

using namespace rapidjson;

class WebParserConfig
{

public:
    struct DataInfo
    {
        std::string id;
        std::string path;
        std::string data_extractor;
        std::string data_parse_function;
    };

    struct WebData
    {
        std::string id;
        std::vector<std::string> mainUrls;
        std::string mainPath;
        std::map<std::string, DataInfo> dataInfo;
        std::vector<std::string> filterFunctions;
        std::string overrideID;
    };

public:

    WebParserConfig(Log* log);
    void Parse(const std::string& path);
    const WebParserConfig::WebData  GetDataInfo(const std::string& str) const;
    std::vector<std::string> GetAllKeys() const;

private:
    bool CanBeFiltered(rapidjson::Value& v);

private:
    std::map< std::string, WebData> _webData;
    Log* _log;
};