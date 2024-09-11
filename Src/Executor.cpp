#include "Executor.h"
#include "DatabaseConnector.h"
#include "WebParserConfig.h"
#include "WebConnector.h"
#include "WebParser.h"
#include "Add.h"
#include "GeneralConfig.h"
#include <thread>

Executor::Executor(GeneralConfig* generalConfig, DatabaseConnector* db, WebConnector* downloader):_db(db), _downloader(downloader), _generalConfig(generalConfig) {}

Executor::~Executor()
{
    _db = nullptr;
    _downloader = nullptr;
    _generalConfig = nullptr;
}

void Executor::Run(WebParserConfig* config)const
{
    if (_generalConfig->GetValueBool("use_threads"))
    {
        RunThreads(config);
    }
    else
    {
        RunIterative(config);
    }
}

void Executor::RunIterative(WebParserConfig* config) const
{
    auto allKeys = config->GetAllKeys();

    for (auto&& key : allKeys)
    {
        ParseData(config, key);
    }
}
void Executor::RunThreads(WebParserConfig* config)const
{
    std::vector<std::thread> allThreads;

    auto allKeys = config->GetAllKeys();

    for (auto&& key : allKeys)
    {
        allThreads.push_back(std::thread (&Executor::ParseData, this, config, key));
    }


    for (auto& t : allThreads)
    {
        t.join();
    }
}


void Executor::ParseData(WebParserConfig* config, std::string key) const
{
    auto webData = config->GetDataInfo(key);

    std::string txt = _downloader->Get(webData.mainUrl);

    WebParser* parser = new WebParser(txt);

    auto result = parser->Parse(webData);

    for (auto&& r : result)
    {
        auto url = r["url"];
        auto price = r["price"];
        auto source = webData.id;

        Add add;
        add.BuildAdd(r);
        add.source = webData.id;

        _db->TryInsertAd(add);
    }
}