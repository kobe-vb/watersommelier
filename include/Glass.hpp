# pragma once

#include "Win.hpp"
#include "Button.hpp"
#include "TextInp.hpp"
#include "Dropdown.hpp"
#include "GameData.hpp"
#include "StackedBar.hpp"

class Glass: public Win
{
private:
    Rectangle rect;
    Dropdown    *name;
    TextInp        *amount;
    GameData    &data;

    std::function<void(UI &)> next_glas_func;

public:
    Glass(GameData &data, std::function<void(UI &)> close_glas);
    ~Glass() = default;

    void draw(void) const override;

    bool take_code(std::string &code) const;

    void save_druple(UI &ui);
    void add_comment(UI &ui);

    std::string get_comment(void) const;
    void reset(void);

public:

    StackedBar  bar;
    float       ph = 0;
    float       mol = 0;
};