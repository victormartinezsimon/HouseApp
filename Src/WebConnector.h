#pragma once

#include <string>
#include <functional>

class WebConnector
{
    using DownloaderCallback = std::function<void(const std::string&)>;

public:
    WebConnector();
    ~WebConnector();

public:
    std::string GetTextFromWeb(const std::string& url) const;

    void GetTelegramChatID(const std::string& telegramBotKey);

    void WriteTelegramMessage(const std::string& telegramBotKey, const std::string& chatID, const std::string& msg);
};