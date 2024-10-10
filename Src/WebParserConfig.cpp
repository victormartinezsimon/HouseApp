#include "WebParserConfig.h"
#include "Log.h"
#include <fstream>        // For read json file
#include <sstream>        // To conver json file to string


WebParserConfig::WebParserConfig(Log* log):_log(log) {}

std::string WebParserConfig::ReadFile(const std::string& path)
{
    std::ifstream file(path);
    if (!file.is_open()) 
    {
        _log->WriteLog("Error reading file: " + path, Log::LOG_TYPE::LOG_FATAL);
    }

    std::stringstream buffer;
    buffer << file.rdbuf();  // Leer el contenido del archivo en el buffer
    return buffer.str();     // Convertir el buffer a string
}

void WebParserConfig::Parse(const std::string& path)
{
    std::string jsonContent = ReadFile(path);

    // Crear el documento JSON
    rapidjson::Document doc;

    // Parsear el JSON
    if (doc.Parse(jsonContent.c_str()).HasParseError()) {
        _log->WriteLog("Error parsing file: " + path, Log::LOG_TYPE::LOG_FATAL);
        return;
    }

    for (auto& v : doc.GetArray())
    {
        if (!v.IsObject())
        {
            _log->WriteLog("Some error while parsing the web data", Log::LOG_TYPE::LOG_FATAL);
        }

        WebData webData;

        if (CanBeFiltered(v))
        {
            _log->WriteLog("Filtered some config!!!", Log::LOG_TYPE::LOG_LOW);
            continue;
        }

        webData.id = v["id"].GetString();
        webData.mainPath = v["main_path"].GetString();

        for (auto& url : v["main_urls"].GetArray())
        {
            std::string url_str = url.GetString();
            webData.mainUrls.push_back(url_str);
        }

        if (v.HasMember("filter_functions"))
        {
            for (auto& fun : v["filter_functions"].GetArray())
            {
                std::string filterFun = fun.GetString();
                webData.filterFunctions.push_back(filterFun);
            }
        }

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
        std::string overrideID = "";
        if (v.HasMember("overrideID"))
        {
            overrideID = v["overrideID"].GetString();
        }
        webData.overrideID = overrideID;

        bool useJavaScript = false;
        if (v.HasMember("useJavaScript"))
        {
            useJavaScript = v["useJavaScript"].GetBool();
        }
        webData.useJavaScript = useJavaScript;

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

bool WebParserConfig::CanBeFiltered(rapidjson::Value& v)
{
    if (v.HasMember("enabled"))
    {
        if (!v["enabled"].GetBool())
        {
            return true;
        }
    }

    if (v.HasMember("available_days"))
    {
        time_t timestamp = time(NULL);
        struct tm datetime;
        localtime_s(&datetime, &timestamp);
        int today = datetime.tm_wday;

        bool anyValidDay = false;
        for (auto& day : v["available_days"].GetArray())
        {
            int validDay = day.GetInt();
            if (validDay == today)
            {
                anyValidDay = true;
            }
        }

        if (!anyValidDay)
        {
            return true;
        }
    }

    return false;
}