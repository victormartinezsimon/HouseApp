#pragma once
#include <string>

class DatabaseConnector;
class WebConnector;
class WebParserConfig;
class GeneralConfig;

class Executor
{

public:
    Executor(GeneralConfig* generalConfig, DatabaseConnector* db, WebConnector* downloader);
    ~Executor();

    void Run(WebParserConfig* config)const;

private:

    void RunIterative(WebParserConfig* config) const;
    void RunThreads(WebParserConfig* config) const;

    void ParseData(WebParserConfig* config, std::string key) const;

    DatabaseConnector* _db;
    WebConnector* _downloader;
    GeneralConfig* _generalConfig;
};