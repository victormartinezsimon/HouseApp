#pragma once
#include <string>
#include <vector>

class DatabaseConnector;
class WebConnector;
class WebParserConfig;
class GeneralConfig;
class Log;

class Executor
{

public:
    Executor(GeneralConfig* generalConfig, DatabaseConnector* db, WebConnector* downloader, Log* log);
    ~Executor();

    std::vector<size_t> Run(WebParserConfig* config)const;

private:

    std::vector<size_t> RunIterative(WebParserConfig* config) const;
    std::vector<size_t> RunThreads(WebParserConfig* config) const;

    std::vector<size_t> ParseData(WebParserConfig* config, std::string key) const;

    DatabaseConnector* _db;
    WebConnector* _downloader;
    GeneralConfig* _generalConfig;
    Log* _log;
};