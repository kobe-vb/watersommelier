
#include "Glass.hpp"

Glass::Glass(int ind, GameData &data) : ind(ind)
{
    add_ui(std::make_unique<Dropdown>(
        100, 200 + ind * 100, 100, 50,
        data.opt, "lol"));
    add_ui(std::make_unique<Text>(300, 200 + ind * 100, 100, 50, nullptr, "hoeveel?"));
    add_ui(std::make_unique<Button>(400, 200 + ind * 100, 100, 50, "save",
        [this](UI& ui) { save_druple(ui); }));

    name = (Dropdown *)get_ui_at(0);
    amount = (Text *)get_ui_at(1);
}

void Glass::save_druple(UI &ui)
{
    name->reset();
    amount->reset();
}
void Glass::draw(void) const
{
    DrawRectangle(500, 200 + ind * 100, 300, 200, RED);
    Win::draw();
}