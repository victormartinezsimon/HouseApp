#include "DatabaseConnector.h"
#include <cassert>

namespace Database
{
    DatabaseConnector::DatabaseConnector()
    {
        Init_database();
        Create_tables();
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
        sql += "PRICE        TEXT     NOT NULL, ";
        sql += "CREATION_DATE DATE     NOT NULL, ";
        sql += "UPDATE_DATE   DATE     NOT NULL, ";
        sql += "AVAILABLE     BOOLEAN ";
        sql += ");";
        
        int result = DB->tryExec(sql);
        assert(SQLite::OK== result);
    }

    void DatabaseConnector::InsertAd(std::string url, std::string source, std::string price)
    {
        std::string sql = "";
        sql += "INSERT INTO ADS (URL,SOURCE,PRICE,CREATION_DATE,UPDATE_DATE,AVAILABLE ) ";
        sql += "VALUES ";
        sql += "(\"" + url + "\"";
        sql += ",\"" + source + "\"";
        sql += ",\"" + price + "\"";
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

        std::string sql = "SELECT ID, URL, SOURCE, AVAILABLE from ADS";
        SQLite::Statement   query(*DB, sql);
        
        while (query.executeStep())
        {
            int id = query.getColumn( 0).getInt();
            const std::string url = query.getColumn(1).getString();
            const std::string source = query.getColumn(2).getString();
            bool available = query.getColumn(2).getInt() == 1;
        
            AdStruct toAdd;
            toAdd.id = id;
            toAdd.url = url;
            toAdd.source = source;
            toAdd.available = available;
        
            toReturn.push_back(toAdd);
        }

        return toReturn;
    }
}
