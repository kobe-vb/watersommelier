

#pragma once

#include "BufferedWin.hpp"
#include "Glass.hpp"
#include "raylib.h"

class HistoryGlass : public BufferedWin
{
public:
    HistoryGlass(int i, Glass &glass);
    ~HistoryGlass() = default;

    void set_pos(int i);

    void draw(void) const override;
    void draww(void) const;

private:

    Rectangle rect;

    int i;
    StackedBar bar;
    std::string comment;
    std::string keyWords;
    float ph = 0;
    float mol = 0;
};