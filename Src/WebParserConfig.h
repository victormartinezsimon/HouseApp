#pragma once
#include <string>
#include <map>

class WebParserConfig
{

public:
    struct DataInfo
    {
        std::string id;
        std::string path;
        std::string data;
    };

    struct WebData
    {
        std::string id;
        std::string mainUrl;
        std::string mainPath;
        std::map<std::string, DataInfo> dataInfo;
    };

public:

    void Parse(const std::string& path);
    const WebData  GetDataInfo(const std::string& str) const;

private:
    std::map< std::string, WebData> _webData;
};