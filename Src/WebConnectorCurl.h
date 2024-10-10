#pragma once

#include <string>
#include <functional>
#include "WebConnector.h"

class Log;

class WebConnectorCurl: public WebConnector
{
public:
    WebConnectorCurl(Log* log);
    ~WebConnectorCurl();

public:
    std::string Get(const std::string& url) const override;

    std::string Post(const std::string& url, const std::string& data) const override;
};