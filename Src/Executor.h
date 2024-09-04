#pragma once
#include <string>

namespace Database
{
    class DatabaseConnector;
}
class WebDownloader;
class WebParserConfig;

class Executor
{

public:
    Executor(Database::DatabaseConnector* db, WebDownloader* downloader);
    ~Executor();

    void Run(WebParserConfig* config);

private:

    void ParseData(WebParserConfig* config, std::string key) const;

    Database::DatabaseConnector* _db;
    WebDownloader* _downloader;
};