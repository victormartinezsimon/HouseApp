#include "WebConnector.h"
#include <curl/curl.h>
#include "Log.h"

#include "rapidjson/document.h"
#include "rapidjson/stringbuffer.h"
#include <rapidjson/writer.h>

using namespace rapidjson;

WebConnector::WebConnector(Log* log):_log(log)
{
    // initialize curl globally
    curl_global_init(CURL_GLOBAL_ALL);
}
WebConnector::~WebConnector()
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

std::string WebConnector::Get(const std::string& url) const
{
    std::string response;
    CURL* handle = curl_easy_init();
    curl_easy_setopt(handle, CURLOPT_CUSTOMREQUEST, "GET");
    curl_easy_setopt(handle, CURLOPT_URL, url.c_str());
    curl_easy_setopt(handle, CURLOPT_WRITEFUNCTION, write_data);
    curl_easy_setopt(handle, CURLOPT_WRITEDATA, &response);
    curl_easy_setopt(handle, CURLOPT_FOLLOWLOCATION, 1L);
    auto res = curl_easy_perform(handle);
    curl_easy_reset(handle);

    if (res != CURLE_OK)
    {
        _log->WriteLog("error in Get: " + url, Log::LOG_TYPE::LOG_FATAL);
    }

    return response;
}

std::string WebConnector::Post(const std::string& url, const std::string& data) const
{
    CURL* handle = curl_easy_init();
    std::string response;

    curl_easy_setopt(handle, CURLOPT_CUSTOMREQUEST, "POST");
    curl_easy_setopt(handle, CURLOPT_URL, url.c_str());
    curl_easy_setopt(handle, CURLOPT_WRITEFUNCTION, write_data);
    curl_easy_setopt(handle, CURLOPT_WRITEDATA, &response);

    struct curl_slist* headers = NULL;
    headers = curl_slist_append(headers, "accept: application/json");
    //headers = curl_slist_append(headers, "User-Agent: Telegram Bot SDK - (https://github.com/irazasyed/telegram-bot-sdk)");
    headers = curl_slist_append(headers, "content-type: application/json");
    curl_easy_setopt(handle, CURLOPT_HTTPHEADER, headers);

    curl_easy_setopt(handle, CURLOPT_POSTFIELDS, data.c_str());

    CURLcode res = curl_easy_perform(handle); // Realiza la solicitud HTTP
    curl_easy_reset(handle);

    if (res != CURLE_OK)
    {
        _log->WriteLog("error in Post: " + url, Log::LOG_TYPE::LOG_FATAL);
    }

    return response;
}
