#pragma once

#include <string>
#include <functional>

class Log;

class WebConnector
{
    using DownloaderCallback = std::function<void(const std::string&)>;

public:
    WebConnector(Log* log);
    ~WebConnector();

public:
    virtual std::string Get(const std::string& url) const;

    std::string Post(const std::string& url, const std::string& data) const;

protected:
    Log* _log;
};