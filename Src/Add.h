#pragma once
#include <string>
#include <map>

class Add
{
public:

    Add();
    void BuildAdd(const std::map<std::string, std::string>& data);
    bool operator==(const Add& otherAdd) const;
    std::string PrintPrettyHtml();

private:
    std::string GetMapValue(const std::map<std::string, std::string>& data, const std::string& key) const;

public:
    std::string url = "";
    std::string source = "";
    std::string price = "";

    const std::string separator = "#####\n";
};