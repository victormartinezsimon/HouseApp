#include <iostream>
#include "DatabaseConnector.h"
#include "WebDownloader.h"


void callbackDownloader(const std::string& str)
{
    std::cout << "callback: " << "\n";
    std::cout << str.substr(0, 100) << "\n";
}

int main()
{
    Database::DatabaseConnector* connector = new Database::DatabaseConnector();
    connector->InsertAd("1", "source_1");
    auto data = connector->GetAllAds();

    for (auto&& d : data)
    {
        std::cout << d.id << "," << d.source << "," << d.url << "\n";
    }
    delete connector;

    WebDownloader* downloader = new WebDownloader();

    auto result = downloader->GetTextFromWeb("https://www.victormartinezsimon.com");
    std::cout << "not thread: " << "\n";
    std::cout << result.substr(0, 100) << "\n";


    downloader->GetTextFromWeb_Thread("https://www.victormartinezsimon.com", callbackDownloader);
    std::this_thread::sleep_for(std::chrono::seconds(5));

    delete downloader;

    return 0;
}