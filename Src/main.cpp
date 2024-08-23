#include <iostream>
#include "DatabaseConnector.h"

#include <curl/curl.h>

size_t write_data(char* data, size_t size, size_t nmemb, void* clientp)
{
    size_t realsize = size * nmemb;
    std::string* mem = (std::string*)clientp;

    mem->append((char*) data, realsize);

    return realsize;
}


std::string testCurl()
{
    // initialize curl globally
    curl_global_init(CURL_GLOBAL_ALL);

    CURL* handle = curl_easy_init();

    curl_easy_setopt(handle, CURLOPT_URL, "https://www.victormartinezsimon.com");
    curl_easy_setopt(handle, CURLOPT_WRITEFUNCTION, write_data);
    std::string result;
    curl_easy_setopt(handle, CURLOPT_WRITEDATA, &result);
    curl_easy_setopt(handle, CURLOPT_FOLLOWLOCATION, 1L);
    auto success = curl_easy_perform(handle);

    curl_easy_reset(handle);

    // free up the global curl resources
    curl_global_cleanup();

    return result;
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

    testCurl();

    return 0;
}