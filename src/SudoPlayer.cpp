#include "SudoPlayer.hpp"

#include <iostream>
#include <winsock2.h>
#include <ws2tcpip.h>

std::string getLocalIP() {
    WSADATA wsaData;
    WSAStartup(MAKEWORD(2, 2), &wsaData);

    SOCKET sock = socket(AF_INET, SOCK_DGRAM, 0);
    sockaddr_in remoteAddr;
    remoteAddr.sin_family = AF_INET;
    remoteAddr.sin_port = htons(80); // willekeurige poort
    inet_pton(AF_INET, "8.8.8.8", &remoteAddr.sin_addr); // Google DNS

    connect(sock, (sockaddr*)&remoteAddr, sizeof(remoteAddr));

    sockaddr_in localAddr;
    int addrLen = sizeof(localAddr);
    getsockname(sock, (sockaddr*)&localAddr, &addrLen);

    char ipStr[INET_ADDRSTRLEN];
    inet_ntop(AF_INET, &localAddr.sin_addr, ipStr, sizeof(ipStr));

    closesocket(sock);
    WSACleanup();

    return std::string(ipStr);
}

SudoPlayer::SudoPlayer(std::string &name, GameData &data, Sim &sim, std::vector<Player_data> &players) :
Player(name, data, sim)
{
    int i = 0;
    for (auto &player : players)
    {
        Players.insert({player.player->get_name(), *player.player});
        add_ui(std::make_unique<Tokel>(1300 + (i % 5) * 120, 100 + (i / 5) * 100, 110, 60, player.player->get_name(), 
                                             [this](UI &uii)
                                         { set_player_website(uii); }));
        i++;
    }
    add_ui(std::make_unique<Tokel>(1300 + (i % 5) * 120, 100 + (i / 5) * 100, 110, 60, this->get_name(),
                                                 [this](UI &uii)
                                         { set_player_website(uii); }));
}

void SudoPlayer::set_player_website(UI &ui)
{
    server.startServer(getLocalIP(), 8080);

    Tokel &tokel = dynamic_cast<Tokel &>(ui);
    Player &player = Players.at(tokel.get_text());
    std::cout << server.getPath(tokel.get_text()) << std::endl;
    player.set_website(server.getPath(tokel.get_text()));
}

void SudoPlayer::update(void)
{
    if (!_is_active)
        return;
    Player::update();
    Win::update();
}

void SudoPlayer::draw(void) const
{
    if (!_is_visible)
        return;
    Player::draw();
    Win::draw();

    DrawText("Role: Sudo", 300, 170, 80, BLACK);
}