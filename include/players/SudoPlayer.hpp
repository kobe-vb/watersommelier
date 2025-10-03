
#pragma once

#include "Player.hpp"
#include "SurveyServer.hpp"

class SudoPlayer : public Player
{
private:

    std::unordered_map<std::string, Player &> players;    
    void set_player_website(UI &ui);
    
protected:
    SurveyServer server;
    void start_server(void);
public:

    SudoPlayer(std::string &name, GameData &data, Sim &sim, std::vector<Player_data> &players);
    ~SudoPlayer() = default;

    bool update(void) override;
    void draw(void) const override;
};

