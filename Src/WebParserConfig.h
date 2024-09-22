#pragma once
#include <string>
#include <map>
#include <vector>

class Log;

class WebParserConfig
{

public:
    struct DataInfo
    {
        std::string id;
        std::string path;
        std::string data_extractor;
    };

    struct WebData
    {
        std::string id;
        std::string mainUrl;
        std::string mainPath;
        std::map<std::string, DataInfo> dataInfo;
    };

public:

    WebParserConfig(Log* log);
    void Parse(const std::string& path);
    const WebData  GetDataInfo(const std::string& str) const;
    std::vector<std::string> GetAllKeys() const;

private:
    std::map< std::string, WebData> _webData;
    Log* _log;
};