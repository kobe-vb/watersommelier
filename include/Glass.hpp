# pragma once

#include "BufferedWin.hpp"
#include "Button.hpp"
#include "TextInp.hpp"
#include "Dropdown.hpp"
#include "GameData.hpp"
#include "StackedBar.hpp"

class Glass: public BufferedWin
{
private:
    int ind;
    Rectangle rect;
    Dropdown    *name;
    TextInp        *amount;
    StackedBar  bar;
    GameData    &data;
    float       ph = 0;
    float       mol = 0;

    std::function<void(UI &)> close_glas_func;

public:
    Glass(int ind, float height, GameData &data, std::function<void(UI &)> close_glas);
    ~Glass() = default;

    void draw(void) const override;

    void save_druple(UI &ui);
    void add_comment(UI &ui);
};