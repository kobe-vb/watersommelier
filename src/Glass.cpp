
#include "Glass.hpp"

Glass::Glass(int ind, GameData &data) : ind(ind)
{
    int x = 100;
    int y = 200 + ind * 300;

    rect = {static_cast<float>(x), static_cast<float>(y), 1200, 100};
    add_ui(std::make_unique<Dropdown>(
        x, y + 10, 400, 50,
        data.opt, "lol"));
    add_ui(std::make_unique<Text>(x + 410, y + 10, 100, 50, [this](UI& ui) { save_druple(ui); }, "hoeveel?"));
    add_ui(std::make_unique<Button>(x + 520, y + 10, 100, 50, "save",
        [this](UI& ui) { save_druple(ui); }));
    
    bar = StackedBar(x + 630, y + 10, 400, 50);
    // bar.add_value("lol", {255, 0, 0, 255}, 5);
    // bar.add_value("dirk", {255, 255, 0, 255}, 10);
    // bar.add_value("dirk", {255, 255, 0, 255}, 10);
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
    DrawRectangleRounded(rect, 0.2, 8, LIGHTGRAY); 
    DrawRectangleRoundedLinesEx(rect, 0.2, 8, 6.0f, BLACK);
    bar.draw();
    Win::draw();
}