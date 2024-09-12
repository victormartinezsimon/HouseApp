#pragma once

#include <SQLiteCpp/SQLiteCpp.h>
#include <string_view>
#include <string>
#include <functional>
#include <vector>

class GeneralConfig;
class Add;

class DatabaseConnector
{
public:
    DatabaseConnector(GeneralConfig* config);
    ~DatabaseConnector();

    bool TryInsertAd(const Add& add, size_t& hashAdded)const;
    std::vector<Add> GetAllAds()const;
    Add GetHashElement(const size_t hash)const;

private:
    void Init_database();
    void Create_tables()const;

    void CreateAdsTable()const;
    size_t GetHash(const Add& add)const;
    void InsertAd(const Add& add, const size_t hash)const;
    bool ManageHashCollision(const Add& add, const size_t hash, int totalCollisions, size_t& hashAdded)const;
    bool IsHashOnDB(const size_t hash)const;
    
    bool TryInsertAd(const Add& add, const size_t hash, int currentCollisions, size_t& hashAdded)const;


private:
    SQLite::Database* DB = nullptr;
    GeneralConfig* _config;
    int _maxCollisions = 10;
    bool _use_hash_collision = false;
    std::string _dataBasePath = "";
};
