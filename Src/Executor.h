#pragma once
#include <string>

class DatabaseConnector;
class WebConnector;
class WebParserConfig;

class Executor
{

public:
    Executor(DatabaseConnector* db, WebConnector* downloader);
    ~Executor();

    void Run(WebParserConfig* config)const;

private:

    void ParseData(WebParserConfig* config, std::string key) const;

    DatabaseConnector* _db;
    WebConnector* _downloader;
};