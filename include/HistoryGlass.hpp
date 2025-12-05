

#pragma once

#include <map>

#include "WebsiteData.hpp"
#include "GameData.hpp"
#include "BufferedWin.hpp"
#include "GlassModel.hpp"
#include "ScoreGlassModel.hpp"
#include "raylib.h"
#include "StackedBarModel.hpp"
#include "StackedBarHView.hpp"

class HistoryGlass : public BufferedWin
{
private:

    UIModel model;

public:
    HistoryGlass(int i, GlassModel &glass, ScoreGlassModel &scoreGlass);
    ~HistoryGlass() = default;

    void set_pos(int i, float scrollOffset);

    void draw(void) const override;
    void draww(void) const;

    void save_data(std::ofstream &file, GameData &data, WebsiteData &websiteData);
    std::string to_json(void) const;

private:

    Rectangle rect;

    int i;
    StackedBarModel bar;
    StackedBarHView bar_view;

    std::string comment;
    std::map<std::string, int> hastags;
    float osmo = 0;
    float volume = 0;
};