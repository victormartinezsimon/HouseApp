#pragma once
#include <string>

namespace Database
{
    class DatabaseConnector;
}
class WebConnector;
class WebParserConfig;

class Executor
{

public:
    Executor(Database::DatabaseConnector* db, WebConnector* downloader);
    ~Executor();

    void Run(WebParserConfig* config)const;

private:

    void ParseData(WebParserConfig* config, std::string key) const;

    Database::DatabaseConnector* _db;
    WebConnector* _downloader;
};