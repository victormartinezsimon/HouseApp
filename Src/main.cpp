#include <iostream>
#include "DatabaseConnector.h"
#include "WebConnector.h"
#include "WebParserConfig.h"
#include "Executor.h"
#include "GeneralConfig.h"
#include "ChatSender.h"
#include "Add.h"

std::string TryGetIDForChat(GeneralConfig* generalConfig, WebConnector* webConnector, ChatSender* chatSender)
{
    std::string chat_key = generalConfig->GetValueString("chat_key");
    bool allowGetChatKey = generalConfig->GetValueBool("allow_get_chat_key");
    std::string telegramKey = generalConfig->GetValueString("telegram_key");

    if (chat_key.empty() && allowGetChatKey )
    {
        chat_key = chatSender->GetChatIDToAllChats();
    }

    return chat_key;
}

void SendNewData(std::vector<size_t>& hashes, DatabaseConnector* db, ChatSender* chatSender, std::string& chatID)
{
    std::string msg = "";

    for (auto&& hash : hashes)
    {
        Add add = db->GetHashElement(hash);

        msg += add.PrintPrettyHtml();
        msg += "\n";
    }

    if (!msg.empty() && !chatID.empty())
    {
        msg = "New Finds: \n" + msg;
        chatSender->SendMessage(chatID, msg);
    }
}


int main()
{
    GeneralConfig* generalConfig = new GeneralConfig();
    generalConfig->Parse("config/general_config.json");

    WebConnector* webConnector = new WebConnector();
    ChatSender* chatSender = new ChatSender(webConnector, generalConfig->GetValueString("telegram_key"));
    
    std::string chat_key = TryGetIDForChat(generalConfig, webConnector, chatSender);
    
    WebParserConfig* config = new WebParserConfig();
    config->Parse(generalConfig->GetValueString("web_data_location"));

    DatabaseConnector* db = new DatabaseConnector( generalConfig );

    Executor* executor = new Executor(generalConfig, db, webConnector);
    auto hashesAdded = executor->Run(config);
    
    SendNewData(hashesAdded, db, chatSender, chat_key);

    return 0;
}