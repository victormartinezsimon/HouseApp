#include "ChatSender.h"
#include "WebConnector.h"
#include <chrono>
#include <thread>

#include <assert.h>
#include "rapidjson/document.h"
#include "rapidjson/stringbuffer.h"
#include <rapidjson/writer.h>
using namespace rapidjson;

ChatSender::ChatSender(WebConnector* connector, const std::string& botID) :_connector(connector), _botID(botID) {}

void ChatSender::GetChatIDToAllChats()const
{
    assert(_botID.size() > 0 && "the bot id is not configurated");

    bool idKnow = false;

    while (!idKnow)
    {
        auto dataDownloaded = _connector->Post(GetUrlForUpdate(), jsonForUpdate);

        if (ParseUpdateResult(dataDownloaded))
        {
            idKnow = true;
        }
        else
        {
            std::this_thread::sleep_for(std::chrono::milliseconds(2000));
        }
    }
}

void ChatSender::SendMessage(const std::string& chatID, const std::string& msg)const
{
    assert(_botID.size() > 0 && "the bot id is not configurated");

    Document d; 
    d.SetObject();

    Value text;
    text.SetString(msg.c_str(), msg.size());

    Value chat_id;
    chat_id.SetString(chatID.c_str(), chatID.size());

    d.AddMember("text", text, d.GetAllocator());
    d.AddMember("chat_id", chat_id, d.GetAllocator());

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

bool ChatSender::ParseUpdateResult(const std::string& str)const
{
    Document doc;
    doc.Parse(str.c_str());

    assert(doc["result"].IsArray());
    auto resultArray = doc["result"].GetArray();

    if (resultArray.Size() != 0)
    {
        for (SizeType i = 0; i < resultArray.Size(); i++)
        {
            auto result = resultArray[i].Get<rapidjson::GenericObject<true, rapidjson::Value>>();
            auto message = result["message"].Get<rapidjson::GenericObject<true, rapidjson::Value>>();
            auto chat = message["chat"].Get<rapidjson::GenericObject<true, rapidjson::Value>>();
            auto id = chat["id"].GetInt64();

            std::string messageToSend = "The ID for this chat is: " + std::to_string(id);

            SendMessage(std::to_string(id), messageToSend);
        }
    }

    return resultArray.Size() != 0;
}