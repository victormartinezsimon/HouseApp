#include "WebParser.h"
#include "WebParserConfig.h"

//code from: https://www.zenrows.com/blog/c-plus-plus-web-scraping#parse-gtml-data




WebParser::WebParser(const std::string& str)
{
    doc = htmlReadMemory(str.c_str(), str.length(), nullptr, nullptr, HTML_PARSE_NOERROR);
    context = xmlXPathNewContext(doc);

    parseFunctions.insert({ "extract_first_number",[this](const WebParserConfig::WebData& webData, const std::string& url, const std::string& value) {return this->extract_first_number(webData, url, value); } });

    parseFunctions.insert({ "join_with_url",[this](const WebParserConfig::WebData& webData, const std::string& url, const std::string& value) {return this->join_with_url(webData, url, value); } });
}

WebParser::~WebParser()
{
    xmlXPathFreeContext(context);
    xmlFreeDoc(doc);
}


std::vector<std::map<std::string, std::string>> WebParser::Parse(const WebParserConfig::WebData& webData, const std::string& url) const
{
    std::vector<std::map<std::string, std::string>> result;

    xmlXPathObjectPtr product_html_elements = xmlXPathEvalExpression((xmlChar*)webData.mainPath.c_str(), context);

    for (int elementIndex = 0; elementIndex < product_html_elements->nodesetval->nodeNr; ++elementIndex)
    {
        // get the current element of the loop
        xmlNodePtr product_html_element = product_html_elements->nodesetval->nodeTab[elementIndex];

        // set the context to restrict XPath selectors
        // to the children of the current element
        xmlXPathSetContextNode(product_html_element, context);

        std::map<std::string, std::string> infoExtracted;

        bool allValid = true;

        for (auto&& param : webData.dataInfo)
        {
            std::string key = param.first;
            auto data = param.second;

            auto path = data.path;
            auto data_extractor = data.data_extractor;

            xmlNodePtr html_element = xmlXPathEvalExpression((xmlChar*)path.c_str(), context)->nodesetval->nodeTab[0];
            if (html_element != nullptr)
            {
                std::string value = "";
                if (data_extractor == "")
                {
                    value = std::string(reinterpret_cast<char*>(xmlNodeGetContent(html_element)));
                }
                else
                {
                    value = std::string(reinterpret_cast<char*>(xmlGetProp(html_element, (xmlChar*)data_extractor.c_str())));
                }

                if (!data.data_parse_function.empty())
                {
                    value = parseFunctions.at(data.data_parse_function)(webData, url, value);
                }

                infoExtracted.insert({ key, value });
            }
            else
            {
                allValid = false;
            }
        }

        if (allValid)
        {
            result.push_back(infoExtracted);
        }
    }

    return result;
}


std::string WebParser::extract_first_number(const WebParserConfig::WebData& webData, const std::string& url, const std::string& value)
{
    char const* digits = "0123456789.";
    std::size_t const n = value.find_first_of(digits);
    if (n != std::string::npos)
    {
        std::size_t const m = value.find_first_not_of(digits, n);
        return value.substr(n, m != std::string::npos ? m - n : m);
    }
    return std::string();
}

std::string WebParser::join_with_url(const WebParserConfig::WebData& webData, const std::string& url, const std::string& value)
{
    return url + value;
}