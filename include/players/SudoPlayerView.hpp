
#pragma once

#include "PlayerView.hpp"
#include "SudoPlayerModel.hpp"
#include "SurveyServer.hpp"

class SudoPlayerView : public PlayerView
{
private:
    int x_text;

    void set_player_website(const std::string &name);

protected:
    SudoPlayerModel *_model;

public:
    SudoPlayerView(PlayerModel *model);
    ~SudoPlayerView() = default;

    bool update(void) override;
    void draw(void) const override;
};
