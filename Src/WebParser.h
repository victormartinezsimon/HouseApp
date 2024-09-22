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

    std::string extract_first_number(std::string const& str);

    xmlXPathContextPtr context;
    htmlDocPtr doc;

    std::map < std::string, std::function<std::string(const std::string&)>> parseFunctions;
};