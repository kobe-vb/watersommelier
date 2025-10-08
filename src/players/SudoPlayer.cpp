#include "SudoPlayer.hpp"

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


SudoPlayer::SudoPlayer(std::string &name, GameData &data, Sim &sim, std::vector<Player_data> &raw_players) :
Player(name, data, sim), server(players)
{
    int i = 0;
    for (auto &player : raw_players)
    {
        players.insert({player.player->get_name(), *player.player});
        add_ui(std::make_unique<Tokel>(1300 + (i % 5) * 120, 100 + (i / 5) * 100, 110, 60, player.player->get_name(), 
                                             [this](UI &uii)
                                         { set_player_website(uii); }));
        i++;
    }
    players.insert({this->get_name(), *this});
    add_ui(std::make_unique<Tokel>(1300 + (i % 5) * 120, 100 + (i / 5) * 100, 110, 60, this->get_name(),
                                                 [this](UI &uii)
                                         { set_player_website(uii); }));
}

void SudoPlayer::start_server(void)
{
    server.startServer(getLocalIP(), 8080);
}

void SudoPlayer::set_player_website(UI &ui)
{
    this->start_server();
    Tokel &tokel = dynamic_cast<Tokel &>(ui);
    Player &player = players.at(tokel.get_text());
    std::cout << server.getPath(tokel.get_text()) << std::endl;
    player.set_website(server.getPath(tokel.get_text()));
}

bool SudoPlayer::update(void)
{
    if (!_is_active)
        return false;
    Player::update();

    return false;
}

void SudoPlayer::draw(void) const
{
    if (!_is_visible)
        return;
    Player::draw();

    DrawText("Role: Sudo", 300, 170, 80, BLACK);
}