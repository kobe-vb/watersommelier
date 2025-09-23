

#include "History.hpp"
#include "Settings.hpp"

History::History(void) : scrollOffset(0), maxScrollOffset(0)
{
    rect.height = GetScreenHeight() - LINE - PEDING;
    rect.width = (GetScreenWidth() * 1 / 3) - PEDING * 2;
    rect.x = (GetScreenWidth() * 1 / 3) - PEDING;
    rect.y = LINE;
}

void History::save_data(std::ofstream &file, size_t &counter, const std::string &name)
{
    for (int i = 0; i < get_num_of_elements(); i++)
    {
        file << counter << ","
        << "datum" << ","
        << name << ","
        << "SsC zout, SsC zoet, SsC zuur, SsC msg,";
        counter++;
        dynamic_cast<HistoryGlass &>(*get_ui_at(i)).save_data(file);
    }
}

void History::saveGlass(Glass &glass)
{
    int i = get_num_of_elements();
    add_ui(std::make_unique<HistoryGlass>(i, glass));

    for (int i = 0; i < get_num_of_elements(); i++)
        dynamic_cast<HistoryGlass &>(*get_ui_at(i)).set_pos(get_num_of_elements() - i - 1, 0);

    calculateMaxScroll();
}

void History::calculateMaxScroll()
{
    int totalHeight = get_num_of_elements() * 140;
    maxScrollOffset = fmax(0, totalHeight - rect.height + PEDING * 8);
}

bool History::updateScroll()
{
    Vector2 mousePos = this->get_mouse_pos();

    if (CheckCollisionPointRec(mousePos, rect))
    {
        float wheelMove = GetMouseWheelMove();
        if (wheelMove != 0)
        {
            scrollOffset -= wheelMove * 30;
            scrollOffset = fmax(0, fmin(scrollOffset, maxScrollOffset));
            return (true);
        }
    }
    return (false);
}

void History::update()
{
    if (updateScroll())
    {
        for (int i = 0; i < get_num_of_elements(); i++)
        {
            HistoryGlass &glass = dynamic_cast<HistoryGlass &>(*get_ui_at(i));
            glass.set_pos(get_num_of_elements() - i - 1, scrollOffset);
        }
    }
}

void History::draw(void) const
{

    for (int i = 0; i < get_num_of_elements(); i++)
        dynamic_cast<HistoryGlass &>(*get_ui_at(i)).draww();
    DrawRectangleRounded(rect, ROUNDED, 10, COL_1);

    BeginScissorMode(rect.x, rect.y, rect.width, rect.height);
    Win::draw();
    EndScissorMode();

    DrawRectangleRoundedLinesEx(rect, ROUNDED, 10, 6.0f, BLACK);
}