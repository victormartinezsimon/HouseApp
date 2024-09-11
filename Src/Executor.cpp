#include "Executor.h"
#include "DatabaseConnector.h"
#include "WebParserConfig.h"
#include "WebConnector.h"
#include "WebParser.h"

Executor::Executor(Database::DatabaseConnector* db, WebConnector* downloader):_db(db), _downloader(downloader) {}


void Executor::Run(WebParserConfig* config)const
{
    auto allKeys = config->GetAllKeys();

    for (auto&& key : allKeys)
    {
        ParseData(config, key);
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

        _db->TryInsertAd(url, source, price);
    }
}