#include "SurveyServer.hpp"
#include <unordered_map>

static std::string loadFile(const std::string &path)
{
    std::ifstream file(path);
    if (!file.is_open())
    {
        std::cerr << "⚠️  Kon bestand niet openen: " << path << std::endl;
        return "";
    }

    std::stringstream buffer;
    buffer << file.rdbuf();
    return buffer.str();
}

SurveyServer::SurveyServer(std::unordered_map<std::string, PlayerModel &> &Players) : players(Players)
{
}

SurveyServer::~SurveyServer()
{
    stopServer();
}

void SurveyServer::serveStaticFile(const std::string &route,
                                   const std::string &file_path,
                                   const std::string &content_type)
{
    server.Get(route.c_str(), [file_path, content_type](const httplib::Request &req, httplib::Response &res)
               {
        (void)req;

        std::string file = loadFile(file_path);
        if (!file.empty()) {
            res.set_content(file, content_type.c_str());
        } else {
            res.status = 404;
            res.set_content("Bestand niet gevonden", "text/plain");
        } });
}

void SurveyServer::setupRoutes()
{

    server.Get("/get-data", [this](const httplib::Request &req, httplib::Response &res)
    {
        auto it = req.params.find("username");
        auto player_it = players.find(it->second);
        // for (auto &player : players)
        //     std::cout << player.first << " " << player.second.to_json() << std::endl;
        if (player_it != players.end())
        {
                res.set_content(player_it->second.to_json().c_str(), "application/json");
                res.status = 200;
        }
        else
        {
            res.status = 404;
            res.set_content("Gebruiker niet gevonden", "text/plain");
        }
    });

server.Post("/send-data", [this](const httplib::Request &req, httplib::Response &res)
{
    if (req.has_header("Content-Type") &&
        req.get_header_value("Content-Type") == "application/x-www-form-urlencoded")
        {

        httplib::Params params;
        httplib::detail::parse_query_text(req.body, params);
                
        auto username_it = params.find("username");
        if (username_it != params.end()) {
            auto player_it = players.find(username_it->second);
            if (player_it != players.end()) {
                PlayerModel& player = player_it->second;

                player.set_data(req.body);
                std::string file = loadFile("./data/www/thx.html");
                if (!file.empty())
                {
                    res.set_content(file, "text/html");
                    res.status = 200;
                }
                else
                {
                    res.status = 404;
                    res.set_content("Bestand niet gevonden", "text/plain");
                }
                return;
            }
        }

        res.status = 404;
        res.set_content("Gebruiker niet gevonden of ongeldige data", "text/plain");
    } else {
        res.status = 400;
        res.set_content("Ongeldig content-type", "text/plain");
    }
});


    // Pre-routing handler for CORS
    server.set_pre_routing_handler([](const httplib::Request &req, httplib::Response &res)
                                   {
        (void)req;
        res.set_header("Access-Control-Allow-Origin", "*");
        res.set_header("Access-Control-Allow-Methods", "GET, POST, OPTIONS");
        res.set_header("Access-Control-Allow-Headers", "Content-Type");
        return httplib::Server::HandlerResponse::Unhandled; });

    // Serve static files
    serveStaticFile("/", "./data/www/form.html", "text/html");
    serveStaticFile("/form.css", "./data/www/form.css", "text/css");
    serveStaticFile("/form.js", "./data/www/form.js", "application/javascript");
    serveStaticFile("/formHelper.js", "./data/www/formHelper.js", "application/javascript");
    serveStaticFile("/cominication.js", "./data/www/cominication.js", "application/javascript");
}

bool SurveyServer::startServer(const std::string &host, int port)
{
    if (is_running)
        return false;

    setupRoutes();

    server_thread = std::thread([this, host, port]()
                                {
        std::cout << "Survey server gestart op http://" << host << ":" << port << std::endl;
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
