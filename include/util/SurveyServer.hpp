#pragma once

#ifndef NOMINMAX
#define NOMINMAX
#endif

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif

#ifndef NOGDI
#define NOGDI
#endif

#ifndef NOUSER
#define NOUSER
#endif
#include "httplib.h"

#include "PlayerModel.hpp"

#include <iostream>
#include <fstream>
#include <thread>
#include <map>
#include <sstream>

class SurveyServer
{
private:
    httplib::Server server;
    std::thread server_thread;
    bool is_running = false;
    
    std::string path;

    std::unordered_map<std::string, PlayerModel &> &players;

    void setupRoutes();
    void serveStaticFile(const std::string& route,
                                   const std::string& file_path,
                                   const std::string& content_type);

public:

    SurveyServer(std::unordered_map<std::string, PlayerModel &> &Players);
    ~SurveyServer();

    void stopServer();
    bool startServer(const std::string& host, int port);

    std::string getPath(const std::string &username);
};
