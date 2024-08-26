#include <iostream>
#include "DatabaseConnector.h"
#include "WebDownloader.h"

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

    std::cout << result.substr(0, 100) << "\n";

    delete downloader;

    return 0;
}