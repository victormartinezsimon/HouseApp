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

        bool TryInsertAd(const std::string& url, const std::string& source, const std::string& price)const;
        std::vector<AdStruct> GetAllAds()const;

    private:
        void Init_database();
        void Create_tables()const;

        void CreateAdsTable()const;
        size_t GetHash(const std::string& url, const std::string& source, const std::string& price)const;
        void InsertAd(const std::string& url, const std::string& source, const std::string& price, const size_t hash)const;
        bool ManageHashCollision(const std::string& url, const std::string& source, const std::string& price, const size_t hash, int totalCollisions)const;
        bool IsHashOnDB(const size_t hash)const;
        AdStruct GetHashElement(const size_t hash)const;
        bool TryInsertAd(const std::string& url, const std::string& source, const std::string& price, const size_t hash, int currentCollisions)const;


    private:
        SQLite::Database* DB = nullptr;
        GeneralConfig* _config;
        int _maxCollisions = 10;
        bool _use_hash_collision = false;
    };
}
