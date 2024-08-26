#include "WebDownloader.h"
#include <curl/curl.h>
#include <future>


WebDownloader::WebDownloader()
{
    // initialize curl globally
    curl_global_init(CURL_GLOBAL_ALL);
}
WebDownloader::~WebDownloader()
{
    // free up the global curl resources
    curl_global_cleanup();
}

size_t write_data(char* data, size_t size, size_t nmemb, void* clientp)
{
    size_t realsize = size * nmemb;
    std::string* mem = (std::string*)clientp;

    mem->append((char*)data, realsize);

    return realsize;
}

std::string WebDownloader::GetTextFromWeb(const std::string& url)
{
    CURL* handle = curl_easy_init();

    curl_easy_setopt(handle, CURLOPT_URL, url.c_str());
    curl_easy_setopt(handle, CURLOPT_WRITEFUNCTION, write_data);
    std::string result;
    curl_easy_setopt(handle, CURLOPT_WRITEDATA, &result);
    curl_easy_setopt(handle, CURLOPT_FOLLOWLOCATION, 1L);
    auto success = curl_easy_perform(handle);
    curl_easy_reset(handle);

    return result;
}
