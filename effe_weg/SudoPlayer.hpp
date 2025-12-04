
#pragma once

#include "PlayerModel.hpp"
#include "SurveyServer.hpp"

class SudoPlayerModel : public PlayerModel
{
private:

    std::unordered_map<std::string, PlayerModel &> players;    
    void set_player_website(UI &ui);
    
protected:
    SurveyServer server;
    void start_server(void);
public:

    SudoPlayerModel(std::string &name, GameData &data, Sim &sim, std::vector<Player_data> &players);
    ~SudoPlayerModel() = default;

    bool update(void) override;
    void draw(void) const override;
};

