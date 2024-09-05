#pragma once

#include <SQLiteCpp/SQLiteCpp.h>
#include <string_view>
#include <string>
#include <functional>
#include <vector>

class GeneralConfig;

namespace Database
{
    constexpr std::string_view databaseFile = "./example.db";

    struct AdStruct
    {
        int id = 0;
        std::string url = "";
        std::string source = "";
        std::string price = "";
        bool available = false;
    };


    class DatabaseConnector
    {
    public:
        DatabaseConnector(GeneralConfig* config);
        ~DatabaseConnector();

        bool TryInsertAd(const std::string& url, const std::string& source, const std::string& price);
        std::vector<AdStruct> GetAllAds();

    private:
        void Init_database();
        void Create_tables();

        void CreateAdsTable();
        size_t GetHash(const std::string& url, const std::string& source, const std::string& price);
        void InsertAd(const std::string& url, const std::string& source, const std::string& price, const size_t hash);
        bool ManageHashCollision(const std::string& url, const std::string& source, const std::string& price, const size_t hash);
        bool IsHashOnDB(const size_t hash);
        AdStruct GetHashElement(const size_t hash);


    private:
        SQLite::Database* DB = nullptr;
        GeneralConfig* _config;
    };
}
