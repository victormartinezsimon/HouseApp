#pragma once
#include "libxml/HTMLparser.h"
#include "libxml/xpath.h"
#include "WebParserConfig.h"

#include <string>
#include <map>
#include <vector>

class Log;

class WebParser
{
public:
    WebParser(const std::string& str, Log* log);
    ~WebParser();

    std::vector<std::map<std::string, std::string>> Parse(const WebParserConfig::WebData& webData, const std::string& url)const;

private:

    std::string extract_first_number(const WebParserConfig::WebData& webData, const std::string& url, const std::string&);
    std::string join_with_url(const WebParserConfig::WebData& webData, const std::string& url, const std::string&);

    xmlXPathContextPtr context;
    htmlDocPtr doc;

    std::map < std::string, std::function<std::string(const WebParserConfig::WebData& webData, const std::string& url, const std::string&)>> parseFunctions;
    Log* _log;
};