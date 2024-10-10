#include "WebConnectorJS.h"
#include "Log.h"

#include <iostream>
#include <memory>
#include <stdexcept>
#include <string>
#include <array>


WebConnectorJS::WebConnectorJS(Log* log):WebConnector(log)
{
    
}
WebConnectorJS::~WebConnectorJS()
{
}

std::string WebConnectorJS::execCommand(const std::string& cmd) const
{
    std::array<char, 128> buffer;
    std::string result;
    std::unique_ptr<FILE, decltype(&_pclose)> pipe(_popen(cmd.c_str(), "r"), _pclose);
    if (!pipe) {
        _log->WriteLog("popen() failed!", Log::LOG_TYPE::LOG_FATAL);
    }
    while (fgets(buffer.data(), buffer.size(), pipe.get()) != nullptr) {
        result += buffer.data();
    }
    return result;
}

std::string WebConnectorJS::Get(const std::string& url) const
{

    // Ajustar el comando según el sistema operativo
#ifdef _WIN32
    std::string python_cmd = "python scripts/scrape.py";
    std::string chromePath = "C:\\Users\\Victor\\Documents\\Workspace\\HouseApp\\Build\\scripts/chromedriver.exe";
#else
    std::string python_cmd = "python3 scripts/scrape.py";
#endif

    // Construir el comando con la URL
    std::string command = python_cmd + " " + url + " " + chromePath;
  
     std::string htmlContent = execCommand(command.c_str());
  
    return htmlContent;
}