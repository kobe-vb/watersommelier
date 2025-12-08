#include "SudoPlayerModel.hpp"

#include <iostream>

#ifdef _WIN32
    #include <winsock2.h>
    #include <ws2tcpip.h>
    #ifdef _MSC_VER
        #pragma comment(lib, "ws2_32.lib")
    #endif
#else
    #include <arpa/inet.h>
    #include <netinet/in.h>
    #include <sys/socket.h>
    #include <unistd.h>
#endif

std::string getLocalIP() {
#ifdef _WIN32
    WSADATA wsaData;
    WSAStartup(MAKEWORD(2, 2), &wsaData);

    SOCKET sock = socket(AF_INET, SOCK_DGRAM, 0);
    sockaddr_in remoteAddr;
    remoteAddr.sin_family = AF_INET;
    remoteAddr.sin_port = htons(80);
    inet_pton(AF_INET, "8.8.8.8", &remoteAddr.sin_addr);

    connect(sock, (sockaddr*)&remoteAddr, sizeof(remoteAddr));

    sockaddr_in localAddr;
    int addrLen = sizeof(localAddr);
    getsockname(sock, (sockaddr*)&localAddr, &addrLen);

    char ipStr[INET_ADDRSTRLEN];
    inet_ntop(AF_INET, &localAddr.sin_addr, ipStr, sizeof(ipStr));

    closesocket(sock);
    WSACleanup();
#else
    int sock = socket(AF_INET, SOCK_DGRAM, 0);
    sockaddr_in remoteAddr{};
    remoteAddr.sin_family = AF_INET;
    remoteAddr.sin_port = htons(80);
    inet_pton(AF_INET, "8.8.8.8", &remoteAddr.sin_addr);

    connect(sock, (sockaddr*)&remoteAddr, sizeof(remoteAddr));

    sockaddr_in localAddr{};
    socklen_t addrLen = sizeof(localAddr);
    getsockname(sock, (sockaddr*)&localAddr, &addrLen);

    char ipStr[INET_ADDRSTRLEN];
    inet_ntop(AF_INET, &localAddr.sin_addr, ipStr, sizeof(ipStr));

    close(sock);
#endif

    return std::string(ipStr);
}

#include "GameModel.hpp"

SudoPlayerModel::SudoPlayerModel(const std::string &name, GameData &data, Sim &sim, GameModel *game) :
PlayerModel(name, data, sim, game), server(players)
{
    role = Role::Sudo;
    for (auto player : game->get_players_ref())
    {
        players.insert({player->get_name(), *player});
        buttons.insert({player->get_name(), TokelModel(player->get_name(), [this, player]() { set_player_website(player->get_name()); })});
    }
        players.insert({get_name(), *this});
        buttons.insert({get_name(), TokelModel(get_name(), [this, name]() { set_player_website(name); })});
}

void SudoPlayerModel::start_server(void)
{
    server.startServer(getLocalIP(), 8080);
}

void SudoPlayerModel::set_player_website(const std::string &name)
{
    std::cout << "set_player_website by: " << name << "" << std::endl;
    this->start_server();

    PlayerModel &player = players.at(name);
    std::cout << server.getPath(name) << std::endl;
    player.set_website(server.getPath(name));
}

