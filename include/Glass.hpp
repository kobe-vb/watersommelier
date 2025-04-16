# pragma once

#include "Win.hpp"
#include "Button.hpp"
#include "Text.hpp"
#include "Dropdown.hpp"
#include "GameData.hpp"

class Glass: public Win
{
private:
    int ind;
    Dropdown    *name;
    Text        *amount;
public:
    Glass(int ind, GameData &data);
    ~Glass() = default;

    void draw(void) const override;

    void save_druple(UI &ui);
};