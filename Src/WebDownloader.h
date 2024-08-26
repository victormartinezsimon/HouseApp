#pragma once

#include <string>
#include <functional>

class WebDownloader
{
    using DownloaderCallback = std::function<std::string()>;

public:
    WebDownloader();
    ~WebDownloader();

public:
    std::string GetTextFromWeb(const std::string& url);
};