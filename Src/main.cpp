#include <iostream>
#include "DatabaseConnector.h"

int main()
{
    Database::DatabaseConnector* connector = new Database::DatabaseConnector();
    connector->InsertAd("1", "source_1");
    auto data = connector->GetAllAds();

    for (auto&& d : data)
    {
        std::cout << d.id << "," << d.source << "," << d.url << "\n";
    }

    return 0;
}