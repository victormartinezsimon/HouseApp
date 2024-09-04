#include "Executor.h"
#include "DatabaseConnector.h"
#include "WebParserConfig.h"
#include "WebDownloader.h"

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
    auto dataInfo = config->GetDataInfo(key);

    std::string txt = _downloader->GetTextFromWeb(dataInfo.mainUrl);

    //Parser(db, dataInfo, txt);

}