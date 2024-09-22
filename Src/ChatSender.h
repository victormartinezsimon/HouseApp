#pragma once
#include <string>

class WebConnector;
class Log;

class ChatSender
{
public:

    ChatSender(WebConnector* connector, const std::string& botID, Log* log);

public:

    std::string GetChatIDToAllChats()const;
    void SendMessage(const std::string& chatID, const std::string& msg)const;

private:
    std::string GetUrlForUpdate() const;
    std::string GetUrlForSendMessage()const;
    bool ParseUpdateResult(const std::string& str, std::string& id)const;

private:

    WebConnector* _connector = nullptr;
    std::string _botID =  "";
    Log* _log;

    const std::string jsonForUpdate = "{\"offset\":null,\"limit\":null,\"timeout\":null}";
};