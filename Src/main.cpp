#include <iostream>
#include "DatabaseConnector.h"
#include "WebConnector.h"
#include "WebParserConfig.h"
#include "Executor.h"
#include "GeneralConfig.h"

int main()
{
    GeneralConfig* generalConfig = new GeneralConfig();
    generalConfig->Parse("config/general_config.json");

    if (generalConfig->GetValueString("chat_key").size() == 0 
        && generalConfig->GetValueBool("allow_get_chat_key") 
        && generalConfig->GetValueString("telegram_key").size() != 0)
    {
        WebConnector* downloader = new WebConnector();
        downloader->GetTelegramChatID(generalConfig->GetValueString("telegram_key"));
    }

    
    WebParserConfig* config = new WebParserConfig();
    config->Parse(generalConfig->GetValueString("web_data_location"));

    Database::DatabaseConnector* db = new Database::DatabaseConnector( generalConfig );
    WebConnector* downloader = new WebConnector();

    Executor* executor = new Executor(db, downloader);
    executor->Run(config);
    
    return 0;
}