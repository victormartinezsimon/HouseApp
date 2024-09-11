#include <iostream>
#include "DatabaseConnector.h"
#include "WebConnector.h"
#include "WebParserConfig.h"
#include "Executor.h"
#include "GeneralConfig.h"
#include "ChatSender.h"

void TryGetIDForChat(GeneralConfig* generalConfig, WebConnector* webConnector, ChatSender* chatSender)
{
    std::string chat_key = generalConfig->GetValueString("chat_key");
    bool allowGetChatKey = generalConfig->GetValueBool("allow_get_chat_key");
    std::string telegramKey = generalConfig->GetValueString("telegram_key");

    if (chat_key.size() == 0
        && allowGetChatKey
        )
    {
        chatSender->GetChatIDToAllChats();
    }
}


int main()
{
    GeneralConfig* generalConfig = new GeneralConfig();
    generalConfig->Parse("config/general_config.json");

    WebConnector* webConnector = new WebConnector();
    ChatSender* chatSender = new ChatSender(webConnector, generalConfig->GetValueString("telegram_key"));
    
    TryGetIDForChat(generalConfig, webConnector, chatSender);
    
    WebParserConfig* config = new WebParserConfig();
    config->Parse(generalConfig->GetValueString("web_data_location"));

    Database::DatabaseConnector* db = new Database::DatabaseConnector( generalConfig );

    Executor* executor = new Executor(db, webConnector);
    executor->Run(config);
    
    return 0;
}