#pragma once
#include "libxml/HTMLparser.h"
#include "libxml/xpath.h"
#include "WebParserConfig.h"

#include <string>
#include <map>
#include <vector>


class WebParser
{
public:
    WebParser(const std::string& str);
    ~WebParser();

    std::vector<std::map<std::string, std::string>> Parse(const WebParserConfig::WebData& webData)const;

private:
    xmlXPathContextPtr context;
    htmlDocPtr doc;
};