

#pragma once

#include <map>

#include "WebsiteData.hpp"
#include "GameData.hpp"
#include "BufferedWin.hpp"
#include "Glass.hpp"
#include "raylib.h"

class HistoryGlass : public BufferedWin
{
public:
    HistoryGlass(int i, Glass &glass);
    ~HistoryGlass() = default;

    void set_pos(int i, float scrollOffset);

    void draw(void) const override;
    void draww(void) const;

    void save_data(std::ofstream &file, GameData &data, WebsiteData &websiteData);
    std::string to_json(void) const;

private:

    Rectangle rect;

    int i;
    StackedBar bar;
    std::string comment;
    std::map<std::string, int> hastags;
    float osmo = 0;
    float volume = 0;
};