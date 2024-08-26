#include <iostream>
#include "DatabaseConnector.h"
#include "WebDownloader.h"

#include "libxml/HTMLparser.h"
#include "libxml/xpath.h"

void TestDataBase()
{
    Database::DatabaseConnector* connector = new Database::DatabaseConnector();
    connector->InsertAd("1", "source_1");
    auto data = connector->GetAllAds();

    for (auto&& d : data)
    {
        std::cout << d.id << "," << d.source << "," << d.url << "\n";
    }
    delete connector;
}


void callbackDownloader(const std::string& str)
{
    std::cout << "callback: " << "\n";
    std::cout << str.substr(0, 100) << "\n";
}
void TestDownloader()
{
    WebDownloader* downloader = new WebDownloader();
    
    auto result = downloader->GetTextFromWeb("https://www.victormartinezsimon.com");
    std::cout << "not thread: " << "\n";
    std::cout << result.substr(0, 100) << "\n";
    
    
    downloader->GetTextFromWeb_Thread("https://www.victormartinezsimon.com", callbackDownloader);
    std::this_thread::sleep_for(std::chrono::seconds(3));
    
    delete downloader;
}

// to store the scraped data of interest
// for each product
struct Product {
    std::string url;
    std::string image;
    std::string name;
    std::string price;
};

void TestParser()
{
    WebDownloader* downloader = new WebDownloader();

    auto result = downloader->GetTextFromWeb("https://www.scrapingcourse.com/ecommerce/");

    htmlDocPtr doc = htmlReadMemory(result.c_str(), result.length(), nullptr, nullptr, HTML_PARSE_NOERROR);

    xmlXPathContextPtr context = xmlXPathNewContext(doc);
    xmlXPathObjectPtr product_html_elements = xmlXPathEvalExpression((xmlChar*)"//li[contains(@class, 'product')]", context);

    std::vector<Product> products;

    for (int i = 0; i < product_html_elements->nodesetval->nodeNr; ++i) {
        // get the current element of the loop
        xmlNodePtr product_html_element = product_html_elements->nodesetval->nodeTab[i];

        // set the context to restrict XPath selectors
        // to the children of the current element
        xmlXPathSetContextNode(product_html_element, context);

        xmlNodePtr url_html_element = xmlXPathEvalExpression((xmlChar*)".//a", context)->nodesetval->nodeTab[0];
        std::string url = std::string(reinterpret_cast<char*>(xmlGetProp(url_html_element, (xmlChar*)"href")));
        xmlNodePtr image_html_element = xmlXPathEvalExpression((xmlChar*)".//a/img", context)->nodesetval->nodeTab[0];
        std::string image = std::string(reinterpret_cast<char*>(xmlGetProp(image_html_element, (xmlChar*)"src")));
        xmlNodePtr name_html_element = xmlXPathEvalExpression((xmlChar*)".//a/h2", context)->nodesetval->nodeTab[0];
        std::string name = std::string(reinterpret_cast<char*>(xmlNodeGetContent(name_html_element)));
        xmlNodePtr price_html_element = xmlXPathEvalExpression((xmlChar*)".//a/span", context)->nodesetval->nodeTab[0];
        std::string price = std::string(reinterpret_cast<char*>(xmlNodeGetContent(price_html_element)));

        Product product = { url, image, name, price };
        products.push_back(product);
    }

    // free up libxml2 resources
    xmlXPathFreeContext(context);
    xmlFreeDoc(doc);


    for (auto p : products)
    {
        std::cout << p.name << "," << p.price << "\n";
    }

}

int main()
{
    TestDataBase();
    
    //TestDownloader();

    TestParser();

    return 0;
}