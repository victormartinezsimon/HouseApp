#include "WebParserConfig.h"
#include "Log.h"

#include "rapidjson/document.h"
#include "rapidjson/stringbuffer.h"
#include "rapidjson/filereadstream.h"

using namespace rapidjson;

WebParserConfig::WebParserConfig(Log* log):_log(log) {}

void WebParserConfig::Parse(const std::string& path)
{
    FILE* fp = fopen(path.c_str(), "r"); // non-Windows use "r"

    char readBuffer[65536];
    FileReadStream is(fp, readBuffer, sizeof(readBuffer));

    Document doc;
    doc.ParseStream(is);


    for (auto& v : doc.GetArray())
    {
        if (!v.IsObject())
        {
            _log->WriteLog("Some error while parsing the web data", Log::LOG_TYPE::LOG_FATAL);
        }

        WebData webData;

        webData.id = v["id"].GetString();
        webData.mainUrl = v["main_url"].GetString();
        webData.mainPath = v["main_path"].GetString();

        const Value& data_to_extract = v["data_to_extract"];
        assert(data_to_extract.IsArray());
        for (SizeType i = 0; i < data_to_extract.Size(); i++)
        {
            auto data = data_to_extract[i].GetObject();

            std::string id_dataInfo = data["id"].GetString();
            std::string path = data["path"].GetString();
            std::string data_extractor = data["data_extractor"].GetString();
            std::string data_parse_function = "";
            if (data.HasMember("data_parse_function"))
            {
                data_parse_function = data["data_parse_function"].GetString();
            }
            webData.dataInfo.insert({ id_dataInfo, { id_dataInfo, path, data_extractor, data_parse_function } });
        }

        _webData.insert({ webData.id , webData });
    }
}

const WebParserConfig::WebData WebParserConfig::GetDataInfo(const std::string& str) const
{
    return _webData.at(str);
}

std::vector<std::string> WebParserConfig::GetAllKeys() const
{
    std::vector<std::string> toReturn;

    for (auto &&v : _webData)
    {
        toReturn.push_back(v.first);
    }
    return toReturn;
}