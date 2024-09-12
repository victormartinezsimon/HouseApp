#include "Executor.h"
#include "DatabaseConnector.h"
#include "WebParserConfig.h"
#include "WebConnector.h"
#include "WebParser.h"
#include "Add.h"
#include "GeneralConfig.h"
#include <thread>
#include <future>

Executor::Executor(GeneralConfig* generalConfig, DatabaseConnector* db, WebConnector* downloader) :_db(db), _downloader(downloader), _generalConfig(generalConfig) {}

Executor::~Executor()
{
    _db = nullptr;
    _downloader = nullptr;
    _generalConfig = nullptr;
}

std::vector<size_t> Executor::Run(WebParserConfig* config)const
{
    if (_generalConfig->GetValueBool("use_threads"))
    {
        return RunThreads(config);
    }
    else
    {
        return RunIterative(config);
    }
}

std::vector<size_t> Executor::RunIterative(WebParserConfig* config) const
{
    auto allKeys = config->GetAllKeys();

    std::vector<size_t> newHashes;

    for (auto&& key : allKeys)
    {
        auto toAdd = ParseData(config, key);

        newHashes.insert(newHashes.end(), toAdd.begin(), toAdd.end());
    }
    return newHashes;
}
std::vector<size_t> Executor::RunThreads(WebParserConfig* config)const
{
    auto allKeys = config->GetAllKeys();

    std::vector<size_t> newHashes;

    std::vector<std::future<std::vector<size_t>>> futuresToWait;

    for (auto&& key : allKeys)
    {
        futuresToWait.push_back(std::async(std::launch::async, &Executor::ParseData, this, config, key));
    }

    for (size_t i = 0; i < futuresToWait.size(); ++i) {
        auto hashesToAdd = futuresToWait[i].get();
        newHashes.insert(newHashes.end(), hashesToAdd.begin(), hashesToAdd.end());
    }
    return newHashes;
}


std::vector<size_t> Executor::ParseData(WebParserConfig* config, std::string key) const
{
    auto webData = config->GetDataInfo(key);

    std::string txt = _downloader->Get(webData.mainUrl);

    WebParser* parser = new WebParser(txt);

    auto result = parser->Parse(webData);

    std::vector<size_t> newHashAdded;

    for (auto&& r : result)
    {
        auto url = r["url"];
        auto price = r["price"];
        auto source = webData.id;

        Add add;
        add.BuildAdd(r);
        add.source = webData.id;

        size_t hashAdded;

        if (_db->TryInsertAd(add, hashAdded))
        {
            newHashAdded.push_back(hashAdded);
        }
    }
    return newHashAdded;
}