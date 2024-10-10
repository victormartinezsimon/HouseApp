#include <iostream>
#include "DatabaseConnector.h"
#include "WebConnector.h"
#include "WebParserConfig.h"
#include "Executor.h"
#include "GeneralConfig.h"
#include "ChatSender.h"
#include "Add.h"
#include "Log.h"

std::string TryGetIDForChat(GeneralConfig* generalConfig, WebConnector* webConnector, ChatSender* chatSender)
{
    std::string chat_key = generalConfig->GetValueString("chat_key");
    bool allowGetChatKey = generalConfig->GetValueBool("allow_get_chat_key");
    std::string telegramKey = generalConfig->GetValueString("telegram_key");

    if (chat_key.empty() && allowGetChatKey )
    {
        chat_key = chatSender->GetChatIDToAllChats();
    }

    return chat_key;
}

void SendNewData(std::vector<size_t>& hashes, DatabaseConnector* db, ChatSender* chatSender, std::string& chatID, Log* log)
{
    std::string msg = "";

    for (auto&& hash : hashes)
    {
        Add add = db->GetHashElement(hash);

        msg += add.PrintPrettyHtml();
        msg += "\n";
    }

    if (!msg.empty() && !chatID.empty())
    {
        log->WriteLog("###Sending new message###", Log::LOG_TYPE::LOG_NORMAL);
        msg = "New Finds: \n" + msg;
        chatSender->SendMessage(chatID, msg);
    }
    else
    {
        log->WriteLog("###Nothing new was found###", Log::LOG_TYPE::LOG_NORMAL);
    }
}

/*
#include "include/cef_app.h"
#include "include/cef_browser.h"
#include "include/cef_command_line.h"
#include "include/cef_client.h"
#include "include/wrapper/cef_helpers.h"

// Un manejador simple que intercepta las solicitudes y guarda el código fuente en una variable.
class SimpleHandler : public CefClient, public CefLifeSpanHandler, public CefRequestHandler, public CefResourceHandler {
public:
    SimpleHandler() {}

    // CefClient methods:
    CefRefPtr<CefLifeSpanHandler> GetLifeSpanHandler() override {
        return this;
    }

    CefRefPtr<CefRequestHandler> GetRequestHandler() override {
        return this;
    }

    // CefLifeSpanHandler methods:
    void OnAfterCreated(CefRefPtr<CefBrowser> browser) override {
        CEF_REQUIRE_UI_THREAD();
        browser_ = browser;
    }

    void OnBeforeClose(CefRefPtr<CefBrowser> browser) override {
        CEF_REQUIRE_UI_THREAD();
        browser_ = nullptr;
    }

    // CefRequestHandler methods:
    CefRefPtr<CefResourceHandler> GetResourceHandler(
        CefRefPtr<CefBrowser> browser,
        CefRefPtr<CefFrame> frame,
        CefRefPtr<CefRequest> request) override {
        CEF_REQUIRE_IO_THREAD();
        return this;
    }
    

    // CefResourceHandler methods:
    void OnResourceLoadComplete(CefRefPtr<CefBrowser> browser,
        CefRefPtr<CefFrame> frame,
        CefRefPtr<CefRequest> request,
        CefRefPtr<CefResponse> response,
        CefResourceHandler::URLRequestStatus status,
        int64 received_content_length) override {
        CEF_REQUIRE_UI_THREAD();

        // Aquí obtenemos el código fuente.
        std::string url = request->GetURL();
        std::cout << "Página cargada: " << url << std::endl;

        // Si es la página principal, guardamos el contenido recibido en la variable de clase.
        if (url == "https://www.example.com/") {
            source_code_ = response_body_;
            std::cout << "Código fuente almacenado en la variable." << std::endl;

            // Cerrar el navegador después de guardar el código fuente.
            browser->GetHost()->CloseBrowser(true);
        }
    }

    bool ReadResponse(void* data_out, int bytes_to_read, int& bytes_read, CefRefPtr<CefCallback> callback) override {
        CEF_REQUIRE_IO_THREAD();

        // Leer la respuesta
        size_t size = std::min(static_cast<size_t>(bytes_to_read), response_body_.size() - read_offset_);
        memcpy(data_out, response_body_.data() + read_offset_, size);
        bytes_read = static_cast<int>(size);
        read_offset_ += size;
        return size > 0;
    }

    void ProcessResponse(CefRefPtr<CefResponse> response, std::string& response_body) {
        int status_code = response->GetStatus();
        std::cout << "Código de respuesta: " << status_code << std::endl;
        if (status_code == 200) {
            // Aquí almacenamos el cuerpo de la respuesta.
            response_body_ = response_body;
        }
    }

    // Obtener el código fuente almacenado.
    std::string GetSourceCode() {
        return source_code_;
    }

private:
    CefRefPtr<CefBrowser> browser_;
    std::string response_body_;
    std::string source_code_; // Aquí almacenamos el código fuente de la página.
    size_t read_offset_ = 0;

    IMPLEMENT_REFCOUNTING(SimpleHandler);
};



int testCEF(const std::string& url)
{
    CefMainArgs main_args;

    CefRefPtr<CefApp> app;

    int exit_code = CefExecuteProcess(main_args, app, nullptr);
    if (exit_code >= 0) {
        return exit_code;
    }

    CefSettings settings;
    settings.no_sandbox = true;

    CefInitialize(main_args, settings, app, nullptr);

    CefWindowInfo window_info;
    window_info.SetAsWindowless(nullptr);//SetAsOffScreen(nullptr);  // No necesitamos ventana gráfica para este caso.

    CefBrowserSettings browser_settings;

    CefRefPtr<SimpleHandler> handler(new SimpleHandler());
    CefBrowserHost::CreateBrowser(window_info, handler, url, browser_settings, nullptr, nullptr);

    // Esperamos a que se cargue la página y obtengamos el código fuente.
    CefRunMessageLoop();

    // Después de que se cierre el navegador, podemos acceder al código fuente.
    std::string source_code = handler->GetSourceCode();

    // Aquí puedes manipular el código fuente como desees.
    std::cout << "Código fuente obtenido:\n" << source_code << std::endl;

    CefShutdown();

    return 0;
}
*/
/*
#include "include/cef_app.h"
#include "include/cef_browser.h"
#include "include/cef_client.h"
#include "include/wrapper/cef_helpers.h"

class SimpleHandler : public CefClient, public CefLifeSpanHandler, public CefLoadHandler {
public:
    SimpleHandler() : is_closing_(false) {}

    // CefClient methods:
    CefRefPtr<CefLifeSpanHandler> GetLifeSpanHandler() override {
        return this;
    }

    CefRefPtr<CefLoadHandler> GetLoadHandler() override {
        return this;
    }

    // CefLifeSpanHandler methods:
    void OnAfterCreated(CefRefPtr<CefBrowser> browser) override {
        CEF_REQUIRE_UI_THREAD();
        browser_ = browser;
    }

    void OnBeforeClose(CefRefPtr<CefBrowser> browser) override {
        CEF_REQUIRE_UI_THREAD();
        if (browser_ && browser_->IsSame(browser)) {
            browser_ = nullptr;
        }
    }

    bool DoClose(CefRefPtr<CefBrowser> browser) override {
        CEF_REQUIRE_UI_THREAD();
        is_closing_ = true;
        return false;  // Permitir que CEF cierre el navegador
    }

    // CefLoadHandler methods:
    void OnLoadEnd(CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame, int httpStatusCode) override {
        CEF_REQUIRE_UI_THREAD();
        // Obtener el código fuente cuando la página se haya cargado
        frame->GetSource(new SourceCodeCallback());
    }

    // Clase interna para capturar el código fuente
    class SourceCodeCallback : public CefStringVisitor {
    public:
        void Visit(const CefString& source) override {
            CEF_REQUIRE_UI_THREAD();
            std::string source_code = source.ToString();
            std::cout << "Source code of the page:\n" << source_code << std::endl;
            CefQuitMessageLoop();  // Terminar el bucle de mensajes después de capturar el código fuente
        }

        void AddRef() const {
            //count = count +1;
        };

        ///
        /// Called to decrement the reference count for the object. Returns true if
        /// the reference count is 0, in which case the object should self-delete.
        ///
        bool Release()
        {
            //--count;
            return count == 0;
        };

        ///
        /// Returns true if the reference count is 1.
        ///
        virtual bool HasOneRef() const
        {
            return count == 1;
        }

        ///
        /// Returns true if the reference count is at least 1.
        ///
        virtual bool HasAtLeastOneRef()
        {
            return count >= 1;
        }

    private:
        int count = 0;
    };

    bool IsClosing() const {
        return is_closing_;
    }

private:
    CefRefPtr<CefBrowser> browser_;
    bool is_closing_;

    IMPLEMENT_REFCOUNTING(SimpleHandler);
};

class SimpleApp : public CefApp, public CefBrowserProcessHandler {
public:
    CefRefPtr<CefBrowserProcessHandler> GetBrowserProcessHandler() override {
        return this;
    }

    void OnContextInitialized() override {
        CEF_REQUIRE_UI_THREAD();

        // Configurar el navegador sin interfaz gráfica (off-screen)
        CefBrowserSettings browser_settings;
        CefWindowInfo window_info;
        window_info.SetAsWindowless(nullptr);  // Sin ventana gráfica

        CefRefPtr<SimpleHandler> handler(new SimpleHandler());

        CefBrowserHost::CreateBrowserSync(window_info, handler.get(), "https://www.google.com", browser_settings, nullptr, nullptr);
    }

private:
    IMPLEMENT_REFCOUNTING(SimpleApp);
};

// Punto de entrada principal
int main(int argc, char* argv[]) {
    // Inicialización de CEF
    CefMainArgs main_args;
    CefRefPtr<CefCommandLine> command_line = CefCommandLine::CreateCommandLine();
    command_line->InitFromArgv(argc, argv);

    CefSettings settings;
    settings.windowless_rendering_enabled = true;  // Habilitar el modo sin ventana

    // Inicializar CEF
    CefRefPtr<SimpleApp> app(new SimpleApp());
    CefInitialize(main_args, settings, app.get(), nullptr);

    // Ejecutar el bucle de eventos de CEF
    CefRunMessageLoop();

    // Limpiar CEF
    CefShutdown();

    return 0;
}
*/

/*
#include "include/cef_app.h"
#include "include/cef_browser.h"
#include "include/cef_client.h"
#include "include/wrapper/cef_helpers.h"
#include <iostream>

class SimpleHandler : public CefClient, public CefLifeSpanHandler, public CefLoadHandler {
public:
    SimpleHandler() : is_closing_(false) {}

    // CefClient methods:
    CefRefPtr<CefLifeSpanHandler> GetLifeSpanHandler() override {
        return this;
    }

    CefRefPtr<CefLoadHandler> GetLoadHandler() override {
        return this;
    }

    // CefLifeSpanHandler methods:
    void OnAfterCreated(CefRefPtr<CefBrowser> browser) override {
        CEF_REQUIRE_UI_THREAD();
        browser_ = browser;
    }

    void OnBeforeClose(CefRefPtr<CefBrowser> browser) override {
        CEF_REQUIRE_UI_THREAD();
        if (browser_ && browser_->IsSame(browser)) {
            browser_ = nullptr;
        }
    }

    bool DoClose(CefRefPtr<CefBrowser> browser) override {
        CEF_REQUIRE_UI_THREAD();
        is_closing_ = true;
        return false;  // Permitir que CEF cierre el navegador
    }

    // CefLoadHandler methods:
    void OnLoadEnd(CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame, int httpStatusCode) override {
        CEF_REQUIRE_UI_THREAD();
        // Obtener el código fuente cuando la página se haya cargado
        frame->GetSource(new SourceCodeCallback());
    }

    // Clase interna para capturar el código fuente
    class SourceCodeCallback : public CefStringVisitor {
    public:
        void Visit(const CefString& source) override {
            CEF_REQUIRE_UI_THREAD();
            std::string source_code = source.ToString();
            std::cout << "Source code of the page:\n" << source_code << std::endl;
            CefQuitMessageLoop();  // Terminar el bucle de mensajes después de capturar el código fuente
        }

        // Implementación de manejo de referencias de CEF
        IMPLEMENT_REFCOUNTING(SourceCodeCallback);
    };

    bool IsClosing() const {
        return is_closing_;
    }

private:
    CefRefPtr<CefBrowser> browser_;
    bool is_closing_;

    IMPLEMENT_REFCOUNTING(SimpleHandler);
};

class SimpleApp : public CefApp, public CefBrowserProcessHandler {
public:
    CefRefPtr<CefBrowserProcessHandler> GetBrowserProcessHandler() override {
        return this;
    }

    void OnContextInitialized() override {
        CEF_REQUIRE_UI_THREAD();

        // Configurar el navegador sin interfaz gráfica (off-screen)
        CefBrowserSettings browser_settings;
        CefWindowInfo window_info;
        window_info.SetAsWindowless(nullptr);  // Sin ventana gráfica

        CefRefPtr<SimpleHandler> handler(new SimpleHandler());

        // Cargar google.com
        CefBrowserHost::CreateBrowserSync(window_info, handler.get(), "https://www.google.com", browser_settings, nullptr, nullptr);
    }

private:
    IMPLEMENT_REFCOUNTING(SimpleApp);
};

// Punto de entrada principal
int main(int argc, char* argv[]) {
    // Inicialización de CEF
    CefMainArgs main_args;
    CefRefPtr<CefCommandLine> command_line = CefCommandLine::CreateCommandLine();
    command_line->InitFromArgv(argc, argv);

    CefSettings settings;
    settings.windowless_rendering_enabled = true;  // Habilitar el modo sin ventana

    // Inicializar CEF
    CefRefPtr<SimpleApp> app(new SimpleApp());
    CefInitialize(main_args, settings, app.get(), nullptr);

    // Ejecutar el bucle de eventos de CEF
    CefRunMessageLoop();

    // Limpiar CEF
    CefShutdown();

    return 0;
}
*/

/*
#include "include/cef_app.h"
#include "include/cef_browser.h"
#include "include/cef_client.h"
#include "include/wrapper/cef_helpers.h"
#include <iostream>
#include <string>

class SimpleHandler : public CefClient, public CefLifeSpanHandler, public CefLoadHandler {
public:
    SimpleHandler() : is_closing_(false) {}

    // CefClient methods:
    CefRefPtr<CefLifeSpanHandler> GetLifeSpanHandler() override {
        return this;
    }

    CefRefPtr<CefLoadHandler> GetLoadHandler() override {
        return this;
    }

    // CefLifeSpanHandler methods:
    void OnAfterCreated(CefRefPtr<CefBrowser> browser) override {
        CEF_REQUIRE_UI_THREAD();
        browser_ = browser;
    }

    void OnBeforeClose(CefRefPtr<CefBrowser> browser) override {
        CEF_REQUIRE_UI_THREAD();
        if (browser_ && browser_->IsSame(browser)) {
            browser_ = nullptr;
        }
    }

    bool DoClose(CefRefPtr<CefBrowser> browser) override {
        CEF_REQUIRE_UI_THREAD();
        is_closing_ = true;
        return false;  // Permitir que CEF cierre el navegador
    }

    // CefLoadHandler methods:
    void OnLoadEnd(CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame, int httpStatusCode) override {
        CEF_REQUIRE_UI_THREAD();
        // Obtener el código fuente cuando la página se haya cargado
        frame->GetSource(new SourceCodeCallback(this));
    }

    // Clase interna para capturar el código fuente
    class SourceCodeCallback : public CefStringVisitor {
    public:
        explicit SourceCodeCallback(SimpleHandler* handler) : handler_(handler) {}

        void Visit(const CefString& source) override {
            CEF_REQUIRE_UI_THREAD();
            std::string source_code = source.ToString();
            handler_->SetSourceCode(source_code);  // Guardar el código fuente en el handler
            CefQuitMessageLoop();  // Terminar el bucle de mensajes después de capturar el código fuente
        }

        // Implementación de manejo de referencias de CEF
        IMPLEMENT_REFCOUNTING(SourceCodeCallback);

    private:
        SimpleHandler* handler_;
    };

    // Método para obtener el código fuente desde main
    std::string GetSourceCode() const {
        return source_code_;
    }

    bool IsClosing() const {
        return is_closing_;
    }

private:
    // Almacenar el código fuente
    void SetSourceCode(const std::string& source) {
        source_code_ = source;
    }

    CefRefPtr<CefBrowser> browser_;
    bool is_closing_;
    std::string source_code_;  // Variable para almacenar el código fuente

    IMPLEMENT_REFCOUNTING(SimpleHandler);
};

class SimpleApp : public CefApp, public CefBrowserProcessHandler {
public:
    SimpleApp(CefRefPtr<SimpleHandler> handler) : handler_(handler) {}

    CefRefPtr<CefBrowserProcessHandler> GetBrowserProcessHandler() override {
        return this;
    }

    void OnContextInitialized() override {
        CEF_REQUIRE_UI_THREAD();

        // Configurar el navegador sin interfaz gráfica (off-screen)
        CefBrowserSettings browser_settings;
        CefWindowInfo window_info;
        window_info.SetAsWindowless(nullptr);  // Sin ventana gráfica

        // Cargar google.com
        CefBrowserHost::CreateBrowserSync(window_info, handler_.get(), "https://www.google.com", browser_settings, nullptr, nullptr);
    }

private:
    CefRefPtr<SimpleHandler> handler_;

    IMPLEMENT_REFCOUNTING(SimpleApp);
};

// Punto de entrada principal
int main(int argc, char* argv[]) {
    // Inicialización de CEF
    CefMainArgs main_args;
    CefRefPtr<CefCommandLine> command_line = CefCommandLine::CreateCommandLine();
    command_line->InitFromArgv(argc, argv);

    CefSettings settings;
    settings.windowless_rendering_enabled = true;  // Habilitar el modo sin ventana

    // Crear el handler
    CefRefPtr<SimpleHandler> handler(new SimpleHandler());

    // Inicializar CEF
    CefRefPtr<SimpleApp> app(new SimpleApp(handler));
    CefInitialize(main_args, settings, app.get(), nullptr);

    // Ejecutar el bucle de eventos de CEF
    CefRunMessageLoop();

    // Obtener el código fuente desde el handler
    std::string source_code = handler->GetSourceCode();
    std::cout << "Final source code:\n" << source_code << std::endl;

    // Limpiar CEF
    CefShutdown();

    return 0;
}
*/


#include <iostream>
#include <memory>
#include <stdexcept>
#include <string>
#include <array>

std::string exec(const char* cmd) {
    std::array<char, 128> buffer;
    std::string result;
    std::unique_ptr<FILE, decltype(&_pclose)> pipe(_popen(cmd, "r"), _pclose);
    if (!pipe) {
        throw std::runtime_error("popen() falló!");
    }
    while (fgets(buffer.data(), buffer.size(), pipe.get()) != nullptr) {
        result += buffer.data();
    }
    return result;
}

int main() {
    // URL de la web
    std::string url = "https://www.fotocasa.es/es/inmobiliaria-servicios-inmobiliarios/comprar/inmuebles/espana/todas-las-zonas/l?clientId=500180019746";

    // Ajustar el comando según el sistema operativo
#ifdef _WIN32
    std::string python_cmd = "python scripts/scrape.py";
#else
    std::string python_cmd = "python3 scripts/scrape.py";
#endif

    std::string chromePath = "C:\\Users\\Victor\\Documents\\Workspace\\HouseApp\\Build\\scripts/chromedriver.exe";

    // Construir el comando con la URL
    std::string command = python_cmd + " " + url+ " " + chromePath;

    try {
        // Ejecuta el script de Selenium y captura la salida
        std::string htmlContent = exec(command.c_str());

        // Imprime el código fuente HTML capturado
        std::cout << "HTML Source Code:\n" << htmlContent << std::endl;
    }
    catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }

    return 0;
}
















/*
int main()
{
    Log* log = new Log();
    log->WriteLog("Start system", Log::LOG_TYPE::LOG_NORMAL);

    log->WriteLog("Reading general config");
    GeneralConfig* generalConfig = new GeneralConfig(log);
    generalConfig->Parse("config/general_config.json");

    WebConnector* webConnector = new WebConnector(log);
    ChatSender* chatSender = new ChatSender(webConnector, generalConfig->GetValueString("telegram_key"), log);
    
    log->WriteLog("Try to get chat key");
    std::string chat_key = TryGetIDForChat(generalConfig, webConnector, chatSender);
    
    log->WriteLog("Read web parser config");
    WebParserConfig* config = new WebParserConfig(log);
    config->Parse(generalConfig->GetValueString("web_data_location"));

    DatabaseConnector* db = new DatabaseConnector( generalConfig, log );

    log->WriteLog("Launching executor");
    Executor* executor = new Executor(generalConfig, db, webConnector, log);
    auto hashesAdded = executor->Run(config);
    
    if (generalConfig->GetValueBool("send_telegram_data", false))
    {
        log->WriteLog("Sending new data");
        SendNewData(hashesAdded, db, chatSender, chat_key, log);
    }
    else
    {
        log->WriteLog("Sending new data is disabled");
        if (hashesAdded.size() != 0)
        {
            log->WriteLog("If eneabled, the amount of hashes sended will be: " + std::to_string(hashesAdded.size()));
        }
        else
        {
            log->WriteLog("If eneabled, no hash will be send." );
        }
    }
    
    log->WriteLog("Finish!!", Log::LOG_TYPE::LOG_NORMAL);
    return 0;
}
*/