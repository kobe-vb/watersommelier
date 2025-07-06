#pragma once

#include "SudoPlayer.hpp"

class WebPlayer : public SudoPlayer
{
private:

public:

    WebPlayer(std::string &name, GameData &data, Sim &sim, std::vector<Player_data> &players);
};

