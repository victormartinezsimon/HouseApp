#include "WebConnector.h"
#include <curl/curl.h>
#include <assert.h>

#include "rapidjson/document.h"
#include "rapidjson/stringbuffer.h"
#include <rapidjson/writer.h>

using namespace rapidjson;

WebConnector::WebConnector()
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
    auto success = curl_easy_perform(handle);
    curl_easy_reset(handle);

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
    assert(res == CURLE_OK);

    return response;
}

void WebConnector::GetTelegramChatID(const std::string& telegramBotKey)
{
    bool messageSend = false;

    const std::string updateURL = "https://api.telegram.org/bot" + telegramBotKey + "/getUpdates";

    while (!messageSend)
    {
        CURL* curl = curl_easy_init();

        std::string dataDownloaded;

        curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "POST");

        curl_easy_setopt(curl, CURLOPT_URL, updateURL.c_str());
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_data);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &dataDownloaded);

        struct curl_slist* headers = NULL;
        headers = curl_slist_append(headers, "accept: application/json");
        //headers = curl_slist_append(headers, "User-Agent: Telegram Bot SDK - (https://github.com/irazasyed/telegram-bot-sdk)");
        headers = curl_slist_append(headers, "content-type: application/json");
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);

        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, "{\"offset\":null,\"limit\":null,\"timeout\":null}");


        CURLcode res = curl_easy_perform(curl); // Realiza la solicitud HTTP
        curl_easy_reset(curl);

        Document doc;
        doc.Parse(dataDownloaded.c_str());

        assert(doc["result"].IsArray());
        auto resultArray = doc["result"].GetArray();

        if (resultArray.Size() != 0)
        {
            for (SizeType i = 0; i < resultArray.Size(); i++)
            {
                auto result = resultArray[i].Get<rapidjson::GenericObject<true, rapidjson::Value>>();
                auto message = result["message"].Get<rapidjson::GenericObject<true, rapidjson::Value>>();
                auto chat = message["chat"].Get<rapidjson::GenericObject<true, rapidjson::Value>>();
                auto id = chat["id"].GetInt64();

                WriteTelegramMessage(telegramBotKey, std::to_string(id), dataDownloaded);
            }
            messageSend = true;
        }
        else
        {
            Sleep(2000);
        }
    }
}

void WebConnector::WriteTelegramMessage(const std::string& telegramBotKey, const std::string& chatID, const std::string& msg)
{
    const std::string messageURL = "https://api.telegram.org/bot" + telegramBotKey + "/sendMessage?chat_id=" + chatID + "&text=" + msg;

    CURL* curl = curl_easy_init();

    if (curl) {
        //curl_easy_setopt(curl, CURLOPT_URL, messageURL.c_str());
        //CURLcode res = curl_easy_perform(curl); 
        //assert(res == CURLE_OK);
        //curl_easy_cleanup(curl);
        CURL* hnd = curl_easy_init();

        curl_easy_setopt(hnd, CURLOPT_CUSTOMREQUEST, "POST");
        curl_easy_setopt(hnd, CURLOPT_WRITEDATA, stdout);
        curl_easy_setopt(hnd, CURLOPT_URL, "https://api.telegram.org/bot7289763318%3AAAHHtviWAGV8DP0p9CgEx51-BwFlzo-hIW0/sendMessage");

        struct curl_slist* headers = NULL;
        headers = curl_slist_append(headers, "accept: application/json");
        headers = curl_slist_append(headers, "User-Agent: Telegram Bot SDK - (https://github.com/irazasyed/telegram-bot-sdk)");
        headers = curl_slist_append(headers, "content-type: application/json");
        curl_easy_setopt(hnd, CURLOPT_HTTPHEADER, headers);

        Document d; // Null
        d.SetObject();
        
        Value text;
        text.SetString(msg.c_str(), msg.size());

        Value chat_id;
        chat_id.SetString(chatID.c_str(), chatID.size());

        //d["text"] = text;
        //d["chat_id"] = chat_id;

        d.AddMember("text", text, d.GetAllocator());
        d.AddMember("chat_id", chat_id, d.GetAllocator());

        StringBuffer buffer;
        Writer<StringBuffer> writer(buffer);
        d.Accept(writer);

        const char* output = buffer.GetString();

        curl_easy_setopt(hnd, CURLOPT_POSTFIELDS, buffer.GetString());

        CURLcode ret = curl_easy_perform(hnd);


    }
}