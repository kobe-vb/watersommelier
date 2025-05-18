


#include "History.hpp"
#include "Settings.hpp"

History::History(void)
{
    rect.height = GetScreenHeight() - LINE - PEDING;
    rect.width = (GetScreenWidth() * 1 / 3) - PEDING * 2;
    rect.x = (GetScreenWidth() * 1 / 3) - PEDING;
    rect.y = LINE;
}

void History::saveGlass(Glass &glass)
{
    // int i = get_num_of_elements();
    // add_ui(std::make_unique<HistoryGlass>(i, glass));

    // for (int i = 0; i < get_num_of_elements(); i++)
    //     dynamic_cast<HistoryGlass&>(*get_ui_at(i)).set_pos(i);
}

void History::draw(void) const
{

    // for (int i = 0; i < get_num_of_elements(); i++)
    //     dynamic_cast<HistoryGlass&>(*get_ui_at(i)).draww();

    BeginScissorMode(rect.x, rect.y, rect.width, rect.height);
    Win::draw();
    EndScissorMode();

    DrawRectangleRounded(rect, ROUNDED, 10, COL_1);
    DrawRectangleRoundedLinesEx(rect, ROUNDED, 10, 6.0f, BLACK);


}