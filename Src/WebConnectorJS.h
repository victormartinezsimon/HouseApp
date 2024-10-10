#pragma once

#include <string>
#include <functional>
#include "WebConnector.h"

class Log;

class WebConnectorJS: public WebConnector
{
    using DownloaderCallback = std::function<void(const std::string&)>;

public:
    WebConnectorJS(Log* log);
    ~WebConnectorJS();

public:
    std::string Get(const std::string& url) const override;

private:
    std::string execCommand(const std::string& cmd) const;
};