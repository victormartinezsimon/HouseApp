#include "Executor.h"
#include "DatabaseConnector.h"
#include "WebParserConfig.h"
#include "WebDownloader.h"
#include "WebParser.h"

Executor::Executor(Database::DatabaseConnector* db, WebDownloader* downloader):_db(db), _downloader(downloader) {}


void Executor::Run(WebParserConfig* config)
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

    std::string txt = _downloader->GetTextFromWeb(webData.mainUrl);

    WebParser* parser = new WebParser(txt);

    auto result = parser->Parse(webData);

    for (auto&& r : result)
    {
        auto url = r["url"];
        auto source = r["price"];

        _db->InsertAd(url, source);
    }
}