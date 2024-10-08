#include "ChatSender.h"
#include "WebConnector.h"
#include "Log.h"
#include <chrono>
#include <thread>


#include "rapidjson/document.h"
#include "rapidjson/stringbuffer.h"
#include <rapidjson/writer.h>
using namespace rapidjson;

ChatSender::ChatSender(WebConnector* connector, const std::string& botID, Log* log) :_connector(connector), _botID(botID), _log(log) {}

std::string ChatSender::GetChatIDToAllChats()const
{
    if (_botID.empty())
    {
        _log->WriteLog("the bot id is not configurated", Log::LOG_TYPE::LOG_ERROR);
        return "";
    }

    bool idKnow = false;

    std::string chatID;

    while (!idKnow)
    {
        auto dataDownloaded = _connector->Post(GetUrlForUpdate(), jsonForUpdate);

        if (ParseUpdateResult(dataDownloaded, chatID))
        {
            idKnow = true;
        }
        else
        {
            std::this_thread::sleep_for(std::chrono::milliseconds(2000));
        }
    }

    return chatID;
}

void ChatSender::SendMessage(const std::string& chatID, const std::string& msg)const
{
    if (_botID.empty())
    {
        _log->WriteLog("the bot id is not configurated", Log::LOG_TYPE::LOG_ERROR);
        return;
    }

    Document d; 
    d.SetObject();

    Value text;
    text.SetString(msg.c_str(), msg.size());

    Value chat_id;
    chat_id.SetString(chatID.c_str(), chatID.size());

    Value parse_mode;
    parse_mode.SetString("html", 4);

    d.AddMember("text", text, d.GetAllocator());
    d.AddMember("chat_id", chat_id, d.GetAllocator());
    d.AddMember("parse_mode", parse_mode, d.GetAllocator());

    StringBuffer buffer;
    Writer<StringBuffer> writer(buffer);
    d.Accept(writer);

    const char* output = buffer.GetString();
    std::string jsonToSend(output);

    _connector->Post(GetUrlForSendMessage(), jsonToSend);
}

std::string ChatSender::GetUrlForUpdate()const
{
    return "https://api.telegram.org/bot" + _botID + "/getUpdates";
}
std::string ChatSender::GetUrlForSendMessage()const
{
    return "https://api.telegram.org/bot" + _botID + "/sendMessage";
}

bool ChatSender::ParseUpdateResult(const std::string& str, std::string& idToReturn)const
{
    Document doc;
    doc.Parse(str.c_str());

    if (!doc["result"].IsArray())
    {
        _log->WriteLog("Result is not an array", Log::LOG_TYPE::LOG_FATAL);
        return false;
    }

    auto resultArray = doc["result"].GetArray();

    if (resultArray.Size() != 0)
    {
        for (SizeType i = 0; i < resultArray.Size(); i++)
        {
            auto result = resultArray[i].Get<rapidjson::GenericObject<true, rapidjson::Value>>();
            auto message = result["message"].Get<rapidjson::GenericObject<true, rapidjson::Value>>();
            auto chat = message["chat"].Get<rapidjson::GenericObject<true, rapidjson::Value>>();
            auto chatID = std::to_string(chat["id"].GetInt64());

            std::string messageToSend = "The ID for this chat is: " + chatID;

            SendMessage(chatID, messageToSend);

            idToReturn = chatID;
        }
    }

    return resultArray.Size() != 0;
}