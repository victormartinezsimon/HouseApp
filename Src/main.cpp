#include <iostream>
#include "DatabaseConnector.h"
#include "WebConnector.h"
#include "WebParserConfig.h"
#include "Executor.h"
#include "GeneralConfig.h"
#include "ChatSender.h"
#include "Add.h"
#include "Log.h"

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

void SendNewData(std::vector<size_t>& hashes, DatabaseConnector* db, ChatSender* chatSender, std::string& chatID, Log* log)
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
        log->WriteLog("###Sending new message###", Log::LOG_TYPE::LOG_NORMAL);
        msg = "New Finds: \n" + msg;
        chatSender->SendMessage(chatID, msg);
    }
    else
    {
        log->WriteLog("###Nothing new was found###", Log::LOG_TYPE::LOG_NORMAL);
    }
}


int main()
{
    Log* log = new Log();
    log->WriteLog("Start system", Log::LOG_TYPE::LOG_NORMAL);

    log->WriteLog("Reading general config");
    GeneralConfig* generalConfig = new GeneralConfig(log);
    generalConfig->Parse("config/general_config.json");

    WebConnector* webConnector = new WebConnector(log);
    ChatSender* chatSender = new ChatSender(webConnector, generalConfig->GetValueString("telegram_key"), log);
    
    log->WriteLog("Try to get chat key");
    std::string chat_key = TryGetIDForChat(generalConfig, webConnector, chatSender);
    
    log->WriteLog("Read web parser config");
    WebParserConfig* config = new WebParserConfig(log);
    config->Parse(generalConfig->GetValueString("web_data_location"));

    DatabaseConnector* db = new DatabaseConnector( generalConfig, log );

    log->WriteLog("Launching executor");
    Executor* executor = new Executor(generalConfig, db, webConnector, log);
    auto hashesAdded = executor->Run(config);
    
    log->WriteLog("Sending new data");
    SendNewData(hashesAdded, db, chatSender, chat_key, log);
    
    log->WriteLog("Finish!!", Log::LOG_TYPE::LOG_NORMAL);
    return 0;
}