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
    void GetTextFromWeb_Thread(const std::string& url, DownloaderCallback callback) const;
};