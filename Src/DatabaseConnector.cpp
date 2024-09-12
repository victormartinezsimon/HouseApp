#include "DatabaseConnector.h"
#include <cassert>
#include "GeneralConfig.h"
#include "Add.h"


DatabaseConnector::DatabaseConnector(GeneralConfig* config):_config(config)
{
    _maxCollisions = _config->GetValueInt("max_collisions");
    _use_hash_collision = _config->GetValueBool("use_hash_collision");
    _dataBasePath = _config->GetValueString("database_path");
    Init_database();
    Create_tables();
}

void DatabaseConnector::Init_database()
{
    DB = new SQLite::Database(_dataBasePath.data(), SQLite::OPEN_READWRITE | SQLite::OPEN_CREATE);
    assert(DB != nullptr);
}

DatabaseConnector::~DatabaseConnector()
{
    if (DB != nullptr)
    {
        delete DB;
        DB = nullptr;
    }
}

void DatabaseConnector::Create_tables()const
{
    CreateAdsTable();
}

void DatabaseConnector::CreateAdsTable()const
{
    std::string sql = "";
    sql += "CREATE TABLE IF NOT EXISTS ADS(";
    sql += "ID INTEGER PRIMARY KEY AUTOINCREMENT, ";
    sql += "SOURCE        TEXT     NOT NULL, ";
    sql += "URL           TEXT     NOT NULL, ";
    sql += "PRICE         TEXT     NOT NULL, ";
    sql += "HASH          TEXT     NOT NULL, ";
    sql += "CREATION_DATE DATE     NOT NULL, ";
    sql += "UPDATE_DATE   DATE     NOT NULL, ";
    sql += "AVAILABLE     BOOLEAN ";
    sql += ");";
        
    int result = DB->tryExec(sql);
    assert(SQLite::OK== result);
}

void DatabaseConnector::InsertAd(const Add& add, const size_t hash)const
{
    std::string sql = "";
    sql += "INSERT INTO ADS (URL,SOURCE,PRICE,HASH,CREATION_DATE,UPDATE_DATE,AVAILABLE ) ";
    sql += "VALUES ";
    sql += "(\"" + add.url + "\"";
    sql += ",\"" + add.source + "\"";
    sql += ",\"" + add.price + "\"";
    sql += ",\"" + std::to_string(hash) + "\"";
    sql += ", DateTime('now')";
    sql += ", DateTime('now')";
    sql += ", TRUE";
    sql += ")";
        
    int result = DB->tryExec(sql);
    assert(SQLite::OK == result);
}

std::vector<Add> DatabaseConnector::GetAllAds()const
{
    std::vector<Add> toReturn;

    std::string sql = "SELECT ID, URL, SOURCE, AVAILABLE, PRICE from ADS";
    SQLite::Statement   query(*DB, sql);
        
    while (query.executeStep())
    {
        int id = query.getColumn( 0).getInt();
        const std::string url = query.getColumn(1).getString();
        const std::string source = query.getColumn(2).getString();
        bool available = query.getColumn(3).getInt() == 1;
        const std::string price = query.getColumn(4).getString();
        
        Add toAdd;
        //toAdd.id = id;
        toAdd.url = url;
        toAdd.source = source;
        //toAdd.available = available;
        toAdd.price = price;
        
        toReturn.push_back(toAdd);
    }

    return toReturn;
}

size_t DatabaseConnector::GetHash(const Add& add)const
{
    std::string str = add.url + "_" + add.source + "_" + add.price;
    auto sol = std::hash<std::string>{}(str);
    return sol;
}

bool DatabaseConnector::TryInsertAd(const Add& add, size_t& hashAdded)const
{
    auto hash = GetHash(add);

    return TryInsertAd(add, hash, _maxCollisions, hashAdded);

}

bool DatabaseConnector::TryInsertAd(const Add& add, const size_t hash, int currentCollisions, size_t& hashAdded)const
{
    if (currentCollisions == 0)
    {
        assert(false && "limit collisions reached");
        return false;
    }

    if (!IsHashOnDB(hash))
    {
        InsertAd(add, hash);
        hashAdded = hash;
        return true;
    }

    if (_use_hash_collision)
    {
        return ManageHashCollision(add, hash, _maxCollisions, hashAdded);
    }

    return false;
}

bool DatabaseConnector::IsHashOnDB(const size_t hash)const
{
    std::string sql = "SELECT * from ADS";
    sql += " where hash = \"" + std::to_string(hash) + "\"";
    SQLite::Statement   query(*DB, sql);

    if (query.executeStep())
    {
        return true;
    }

    return false;
}
   
Add DatabaseConnector::GetHashElement(const size_t hash)const
{
    std::string sql = "SELECT ID, URL, SOURCE, AVAILABLE, PRICE from ADS";
    sql += " where hash = \"" + std::to_string(hash) + "\"";
    SQLite::Statement   query(*DB, sql);

    Add toReturn;

    if (query.executeStep())
    {
        int id = query.getColumn(0).getInt();
        const std::string url = query.getColumn(1).getString();
        const std::string source = query.getColumn(2).getString();
        bool available = query.getColumn(3).getInt() == 1;
        const std::string price = query.getColumn(4).getString();

        //toReturn.id = id;
        toReturn.url = url;
        toReturn.source = source;
        //toReturn.available = available;
        toReturn.price = price;
    }
    else
    {
        assert(false && "hash element should exist. Search before call this method");
    }
    return toReturn;
}

bool DatabaseConnector::ManageHashCollision(const Add& add, const size_t hash, int totalCollisions, size_t& hashAdded)const
{
    auto onDB = GetHashElement(hash);

    if (add == onDB)
    {
        return false;
    }

    size_t newHash = hash + 1;
    return TryInsertAd(add, newHash, totalCollisions - 1, hashAdded);
}

