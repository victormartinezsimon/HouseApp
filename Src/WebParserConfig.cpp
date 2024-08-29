#include "WebParserConfig.h"

#include "rapidjson/document.h"
#include "rapidjson/stringbuffer.h"
#include "rapidjson/filereadstream.h"

using namespace rapidjson;

void WebParserConfig::Parse(const std::string& path)
{

    FILE* fp = fopen(path.c_str(), "r"); // non-Windows use "r"

    char readBuffer[65536];
    FileReadStream is(fp, readBuffer, sizeof(readBuffer));

    Document doc;
    doc.ParseStream(is);


    for (auto& v : doc.GetArray())
    {
        assert(v.IsObject());

        WebData webData;

        webData.mainUrl = v["main_url"].GetString();
        webData.mainPath = v["main_path"].GetString();

        const Value& data_to_extract = v["data_to_extract"];
        assert(data_to_extract.IsArray());
        for (SizeType i = 0; i < data_to_extract.Size(); i++)
        {
            auto data = data_to_extract[i].GetObject();

            std::string id = data["id"].GetString();
            std::string path = data["path"].GetString();
            std::string data_extractor = data["data_extractor"].GetString();
            webData.dataInfo.insert({ id, { id, path, data_extractor } });
        }

        _webData.insert({ webData.mainUrl , webData });
    }
}

const WebParserConfig::WebData WebParserConfig::GetDataInfo(const std::string& str) const
{
    return _webData.at(str);
}