#include "Executor.h"
#include "DatabaseConnector.h"
#include "WebParserConfig.h"
#include "WebConnector.h"
#include "WebParser.h"
#include "Add.h"
#include "GeneralConfig.h"
#include <thread>
#include <future>
#include "Log.h"

Executor::Executor(GeneralConfig* generalConfig, DatabaseConnector* db, WebConnector* downloader, Log* log) :_db(db), _downloader(downloader), _generalConfig(generalConfig),_log(log)
{
    filterFunctions.insert({ "only_leganesNorte", [this](const Add* str) {return this->OnlyLeganesNorteFilter(str); } });
    filterFunctions.insert({ "big_prizes", [this](const Add* str) {return this->BigPrizesFilter(str); } });
}

Executor::~Executor()
{
    _db = nullptr;
    _downloader = nullptr;
    _generalConfig = nullptr;
    _log = nullptr;
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


std::vector<size_t> Executor::ParseData(WebParserConfig* config, const std::string& key) const
{
    _log->WriteLog("searching for: " + key);

    auto webData = config->GetDataInfo(key);

    std::vector<size_t> newHashAdded;
    for (auto&& url : webData.mainUrls)
    {
        auto newHashesTemp = ParseDataWithUrl(config, key, url);
        newHashAdded.insert(newHashAdded.end(), newHashesTemp.begin(), newHashesTemp.end());
    }

    _log->WriteLog("finish searching for: " + key);

    return newHashAdded;
}

std::vector<size_t> Executor::ParseDataWithUrl(WebParserConfig* config, const std::string& key, const std::string& url) const
{
    auto webData = config->GetDataInfo(key);

    std::string txt = _downloader->Get(url);

    WebParser* parser = new WebParser(txt);

    auto result = parser->Parse(webData, url);

    std::vector<size_t> newHashAdded;

    for (auto&& r : result)
    {
        auto url = r["url"];
        auto price = r["price"];
        auto source = webData.id;

        Add add;
        add.BuildAdd(r);
        add.source = webData.id;

        bool filtersOK = true;
        for (auto fun : webData.filterFunctions)
        {
            if (!filterFunctions.at(fun)(&add))
            {
                filtersOK = false;
                break;
            }
        }

        if (!filtersOK)
        {
            continue;
        }

        size_t hashAdded;
        if (_db->TryInsertAd(add, hashAdded))
        {
            newHashAdded.push_back(hashAdded);
        }
    }

    return newHashAdded;
}

bool Executor::OnlyLeganesNorteFilter(const Add* add)
{
    return add->url.find("norte") != std::string::npos;
}
bool Executor::BigPrizesFilter(const Add* add)
{
    float priceInt = atof(add->price.c_str());

    return priceInt > 200.000;
}