#pragma once

#include <string>
#include <functional>
#include "WebConnector.h"

class Log;

class WebConnectorJS: public WebConnector
{
public:
    WebConnectorJS(Log* log);
    ~WebConnectorJS();

public:
    std::string Get(const std::string& url) const override;
    std::string Post(const std::string& url, const std::string& data) const override;

private:
    std::string execCommand(const std::string& cmd) const;
};