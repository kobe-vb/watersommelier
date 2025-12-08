
#pragma once

#include "PlayerModel.hpp"
#include "SurveyServer.hpp"

class SudoPlayerModel : public PlayerModel
{
private:

    std::unordered_map<std::string, PlayerModel &> players;    
    std::unordered_map<std::string, TokelModel> buttons;    
    
    protected:
    SurveyServer server;
    void start_server(void);
    public:
    
    SudoPlayerModel(const std::string &name, GameData &data, Sim &sim, GameModel *game);
    ~SudoPlayerModel() = default;
    
    void set_player_website(const std::string &name);
    
    std::unordered_map<std::string, TokelModel> &get_buttons(void) { return buttons; }  
};

