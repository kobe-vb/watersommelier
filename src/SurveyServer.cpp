#include "SurveyServer.hpp"

static std::string loadFile(const std::string &path)
{
    std::ifstream file(path);
    if (file.is_open())
    {
        std::stringstream buffer;
        buffer << file.rdbuf();
        return buffer.str();
    }
    return "";
}

SurveyServer::SurveyServer() {}

SurveyServer::~SurveyServer()
{
    stopServer();
}

void SurveyServer::setupRoutes()
{
    server.Get("/", [this](const httplib::Request &req, httplib::Response &res)
               {
                (void)req;
                (void)this;
                std::string file = loadFile("./data/www/Form.html");
                if (!file.empty())
                {
                    res.set_content(file, "text/html");
                    return;
                }
                res.status = 404;
                res.set_content("HTML file not found", "text/plain"); });

    server.Get("/Form.css", [](const httplib::Request &req, httplib::Response &res)
               {
                                (void)req;

                std::string file = loadFile("./data/www/Form.css");
                if (!file.empty())
                {
                    res.set_content(file, "text/css");
                    return;
                }
                res.status = 404;
                res.set_content("css file not found", "text/plain"); });

    server.Get("/Form.js", [](const httplib::Request &req, httplib::Response &res)
               {
                                (void)req;

                std::string file = loadFile("./data/www/Form.js");
                if (!file.empty())
                {
                    res.set_content(file, "application/javascript");
                    return;
                }
                res.status = 404;
                res.set_content("js file not found", "text/plain"); });

    server.Post("/api/submit", [this](const httplib::Request &req, httplib::Response &res)
                {
                                    (void)this;

                std::cout << "Antwoord ontvangen: " << req.body << std::endl;
                res.body = "OK";
                res.status = 200; });

    server.set_pre_routing_handler([](const httplib::Request &req, httplib::Response &res)
                                   {
                                                    (void)req;

            res.set_header("Access-Control-Allow-Origin", "*");
            res.set_header("Access-Control-Allow-Methods", "GET, POST, OPTIONS");
            res.set_header("Access-Control-Allow-Headers", "Content-Type");
            return httplib::Server::HandlerResponse::Unhandled; });
}

bool SurveyServer::startServer(const std::string &host, int port)
{
    if (is_running)
        return false;
    setupRoutes();

    server_thread = std::thread([this, host, port]()
                                {
            std::cout << "🌐 Survey server gestart op http://" << host << ":" << port << std::endl;
            server.listen(host.c_str(), port); });
    path = "http://" + host + ":" + std::to_string(port);
    is_running = true;
    return true;
}

void SurveyServer::stopServer()
{
    if (!is_running)
        return;
    std::cout << "🛑 Server wordt gestopt..." << std::endl;
    server.stop();

    if (server_thread.joinable())
        server_thread.join();

    is_running = false;
    std::cout << "✓ Server gestopt" << std::endl;
}

std::string SurveyServer::getPath(const std::string &username)
{
    return path + "?username=" + username;
}