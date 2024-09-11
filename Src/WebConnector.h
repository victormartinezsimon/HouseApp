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
    std::string Get(const std::string& url) const;

    std::string Post(const std::string& url, const std::string& data) const;

    void GetTelegramChatID(const std::string& telegramBotKey);

    void WriteTelegramMessage(const std::string& telegramBotKey, const std::string& chatID, const std::string& msg);
};