#include "WebsitePlayer.hpp"

WebPlayer::WebPlayer(std::string &name, GameData &data, Sim &sim, std::vector<Player_data> &players):
SudoPlayer(name, data, sim, players)
{
    this->demo();
    this->start_server();
    
    std::thread([this]() {
    std::this_thread::sleep_for(std::chrono::milliseconds(500));
    system(("start " + this->server.getPath(this->name)).c_str());
}).detach();

}
