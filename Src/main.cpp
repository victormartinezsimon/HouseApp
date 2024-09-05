#include <iostream>
#include "DatabaseConnector.h"
#include "WebDownloader.h"
#include "WebParserConfig.h"
#include "Executor.h"
#include "GeneralConfig.h"


int main()
{
    GeneralConfig* generalConfig = new GeneralConfig();
    generalConfig->Parse("config/general_config.json");

    WebParserConfig* config = new WebParserConfig();
    config->Parse("config/web_data.json");

    Database::DatabaseConnector* db = new Database::DatabaseConnector();
    WebDownloader* downloader = new WebDownloader();

    Executor* executor = new Executor(db, downloader);
    executor->Run(config);

    return 0;
}