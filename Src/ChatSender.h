#pragma once
#include <string>

class WebConnector;

class ChatSender
{
public:

    ChatSender(WebConnector* connector, const std::string& botID);

public:

    void GetChatIDToAllChats();

    void SendMessage(const std::string& chatID, const std::string& msg);

private:
    std::string GetUrlForUpdate();
    std::string GetUrlForSendMessage();
    bool ParseUpdateResult(const std::string& str);

private:

    WebConnector* _connector;
    std::string _botID;

    const std::string jsonForUpdate = "{\"offset\":null,\"limit\":null,\"timeout\":null}";
};