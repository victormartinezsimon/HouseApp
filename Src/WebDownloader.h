#pragma once

#include <string>
#include <functional>

class WebDownloader
{
    using DownloaderCallback = std::function<void(const std::string&)>;

public:
    WebDownloader();
    ~WebDownloader();

public:
    std::string GetTextFromWeb(const std::string& url) const;

    void GetTelegramChatID(const std::string& telegramBotKey);

    void WriteTelegramMessage(const std::string& telegramBotKey, const std::string& chatID, const std::string& msg);
};