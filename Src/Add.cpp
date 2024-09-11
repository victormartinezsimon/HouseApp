#include "Add.h"

Add::Add() {};
void Add::BuildAdd(const std::map<std::string, std::string>& data)
{
    url = GetMapValue(data, "url");
    price = GetMapValue(data, "price");
    source = GetMapValue(data, "id");
}

std::string Add::GetMapValue(const std::map<std::string, std::string>& data, const std::string& key) const
{
    if (data.find("url") != data.end())
    {
        return data.at("url");;
    }

    return "";
}