#pragma once

#include <string>
#include <functional>

class Log;

class WebConnector
{
public:
    WebConnector(Log* log) :_log(log) {}
    virtual ~WebConnector() {}

public:
    virtual std::string Get(const std::string& url) const { return ""; }

    virtual std::string Post(const std::string& url, const std::string& data) const { return ""; }

protected:
    Log* _log;
};