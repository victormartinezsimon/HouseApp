#pragma once
#include <string>
#include <vector>
#include <map>
#include <functional>

class DatabaseConnector;
class WebConnector;
class WebParserConfig;
class GeneralConfig;
class Log;
class Add;

class Executor
{

public:
    Executor(GeneralConfig* generalConfig, DatabaseConnector* db, WebConnector* downloader, Log* log);
    ~Executor();

    std::vector<size_t> Run(WebParserConfig* config)const;

private:

    std::vector<size_t> RunIterative(WebParserConfig* config) const;
    std::vector<size_t> RunThreads(WebParserConfig* config) const;

    std::vector<size_t> ParseData(WebParserConfig* config, const std::string& key) const;
    std::vector<size_t> ParseDataWithUrl(WebParserConfig* config, const std::string& key, const std::string& url) const;

    bool OnlyLeganesNorteFilter(const Add* str);
    bool BigPrizesFilter(const Add* str);
    bool AnyLeganesFilter(const Add* str);

    std::string to_lower(const std::string& str);

    DatabaseConnector* _db;
    WebConnector* _downloader;
    GeneralConfig* _generalConfig;
    Log* _log;

    std::map<std::string, std::function<bool(const Add* )>> filterFunctions;
};