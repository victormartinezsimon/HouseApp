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
    if (data.find(key) != data.end())
    {
        return data.at(key);;
    }

    return "";
}

bool Add::operator==(const Add& otherAdd) const
{
    return otherAdd.url == url && otherAdd.price == price && otherAdd.source == source;
}

std::string Add::PrintPrettyHtml()
{
    std::string result = "";

    if (!url.empty())
    {
        result += "<b>url: </b>" + url +"\n";
    }

    if (!price.empty())
    {
        result += "<b>price: </b>" + price + "\n";
    }

    if (!source.empty())
    {
        result += "<b>source: </b>" + source + "\n";
    }
    
    if (!result.empty())
    {
        result = separator + result + separator;
    }

    return result;
}