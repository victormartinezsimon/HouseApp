#pragma once

#include <SQLiteCpp/SQLiteCpp.h>
#include <string_view>
#include <string>
#include <functional>
#include <vector>


namespace Database
{
    constexpr std::string_view databaseFile = "./example.db";

    struct AdStruct
    {
        int id = 0;
        std::string url = "";
        std::string source = "";
        bool available = false;
    };


    class DatabaseConnector
    {
    public:
        DatabaseConnector();
        ~DatabaseConnector();

        void InsertAd(std::string url, std::string source);
        std::vector<AdStruct> GetAllAds();

    private:
        void Init_database();
        void Create_tables();

        void CreateAdsTable();


    private:
        SQLite::Database* DB = nullptr;
    };
}
