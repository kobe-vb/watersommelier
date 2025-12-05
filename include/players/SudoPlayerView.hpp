
#pragma once

#include "PlayerView.hpp"
#include "SudoPlayerModel.hpp"
#include "SurveyServer.hpp"

class SudoPlayerView : public PlayerView
{
protected:

    SudoPlayerModel *_model;

public:

    SudoPlayerView(PlayerModel *model);
    ~SudoPlayerView() = default;

    bool update(void) override;
    void draw(void) const override;
};

