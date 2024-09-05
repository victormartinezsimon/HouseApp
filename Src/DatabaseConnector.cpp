#include "DatabaseConnector.h"
#include <cassert>
#include "GeneralConfig.h"

namespace Database
{
    DatabaseConnector::DatabaseConnector(GeneralConfig* config):_config(config)
    {
        Init_database();
        Create_tables();
        _maxCollisions = _config->GetValueInt("max_collisions");
        _use_hash_collision = _config->GetValueBool("use_hash_collision");
    }

    void DatabaseConnector::Init_database()
    {
        DB = new SQLite::Database(databaseFile.data(), SQLite::OPEN_READWRITE | SQLite::OPEN_CREATE);
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

    void DatabaseConnector::Create_tables()
    {
        CreateAdsTable();
    }

    void DatabaseConnector::CreateAdsTable()
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

    void DatabaseConnector::InsertAd(const std::string& url, const std::string& source, const std::string& price, const size_t hash)
    {
        std::string sql = "";
        sql += "INSERT INTO ADS (URL,SOURCE,PRICE,HASH,CREATION_DATE,UPDATE_DATE,AVAILABLE ) ";
        sql += "VALUES ";
        sql += "(\"" + url + "\"";
        sql += ",\"" + source + "\"";
        sql += ",\"" + price + "\"";
        sql += ",\"" + std::to_string(hash) + "\"";
        sql += ", DateTime('now')";
        sql += ", DateTime('now')";
        sql += ", TRUE";
        sql += ")";
        
        int result = DB->tryExec(sql);
        assert(SQLite::OK == result);
    }

    std::vector<AdStruct> DatabaseConnector::GetAllAds()
    {
        std::vector<AdStruct> toReturn;

        std::string sql = "SELECT ID, URL, SOURCE, AVAILABLE, PRICE from ADS";
        SQLite::Statement   query(*DB, sql);
        
        while (query.executeStep())
        {
            int id = query.getColumn( 0).getInt();
            const std::string url = query.getColumn(1).getString();
            const std::string source = query.getColumn(2).getString();
            bool available = query.getColumn(3).getInt() == 1;
            const std::string price = query.getColumn(4).getString();
        
            AdStruct toAdd;
            toAdd.id = id;
            toAdd.url = url;
            toAdd.source = source;
            toAdd.available = available;
            toAdd.price = price;
        
            toReturn.push_back(toAdd);
        }

        return toReturn;
    }

    size_t DatabaseConnector::GetHash(const std::string& url, const std::string& source, const std::string& price)
    {
        std::string str = url + "_" + source + "_" + price;
        auto sol = std::hash<std::string>{}(str);
        return sol;
    }

    bool DatabaseConnector::TryInsertAd(const std::string& url, const std::string& source, const std::string& price)
    {
        auto hash = GetHash(url, source, price);

        return TryInsertAd(url, source, price, hash, _maxCollisions);

    }

    bool DatabaseConnector::TryInsertAd(const std::string& url, const std::string& source, const std::string& price, const size_t hash, int currentCollisions)
    {
        if (currentCollisions == 0)
        {
            assert(false && "limit collisions reached");
            return false;
        }

        if (!IsHashOnDB(hash))
        {
            InsertAd(url, source, price, hash);
            return true;
        }

        if (_use_hash_collision)
        {
            return ManageHashCollision(url, source, price, hash, _maxCollisions);
        }

        return false;
    }

    bool DatabaseConnector::IsHashOnDB(const size_t hash)
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
   
    AdStruct DatabaseConnector::GetHashElement(const size_t hash)
    {
        std::string sql = "SELECT ID, URL, SOURCE, AVAILABLE, PRICE from ADS";
        sql += " where hash = \"" + std::to_string(hash) + "\"";
        SQLite::Statement   query(*DB, sql);

        AdStruct toReturn;

        if (query.executeStep())
        {
            int id = query.getColumn(0).getInt();
            const std::string url = query.getColumn(1).getString();
            const std::string source = query.getColumn(2).getString();
            bool available = query.getColumn(3).getInt() == 1;
            const std::string price = query.getColumn(4).getString();

            toReturn.id = id;
            toReturn.url = url;
            toReturn.source = source;
            toReturn.available = available;
            toReturn.price = price;
        }
        else
        {
            assert(false && "hash element should exist. Search before call this method");
        }
        return toReturn;
    }

    bool DatabaseConnector::ManageHashCollision(const std::string& url, const std::string& source, const std::string& price, const size_t hash, int totalCollisions)
    {
        auto onDB = GetHashElement(hash);

        bool same = true;

        same &= (onDB.url == url);
        same &= (onDB.source == source);
        same &= (onDB.price == price);

        size_t newHash = hash + 1;

        if (same)
        {
            return false;
        }

        return TryInsertAd(url, source, price, newHash, totalCollisions - 1);
    }
}
