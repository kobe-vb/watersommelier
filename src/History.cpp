#include <chrono>
// #include <format>

#include "History.hpp"
#include "Settings.hpp"

History::History(void) : scrollOffset(0), maxScrollOffset(0)
{
    rect.height = GetScreenHeight() - LINE - PEDING;
    rect.width = (GetScreenWidth() * 1 / 3) - PEDING * 2;
    rect.x = (GetScreenWidth() * 1 / 3) - PEDING;
    rect.y = LINE;
}

void History::save_data(std::ofstream &file, size_t &counter, const std::string &name, GameData &data, WebsiteData &websiteData)
{
    auto now = std::chrono::system_clock::now();
    std::time_t t = std::chrono::system_clock::to_time_t(now);
    std::tm tm = *std::localtime(&t);

    // std::string datum = std::format("{:02d}-{:02d}-{:04d}",
    //                                 tm.tm_mday,
    //                                 tm.tm_mon + 1,
    //                                 tm.tm_year + 1900);
    std::string datum = std::to_string(tm.tm_mday) + "-" +
                        std::to_string(tm.tm_mon + 1) + "-" +
                        std::to_string(tm.tm_year + 1900);

    websiteData.set_begin_index(counter);
    for (int i = 0; i < get_num_of_elements(); i++)
    {
        file << counter++ << ";"
        << datum << ";"
        << name << ";"
        << websiteData.get("zoet") << ";"
        << websiteData.get("zout") << ";"
        << websiteData.get("zuur") << ";"
        << websiteData.get("bitter") << ";"
        << websiteData.get("umami") << ";";

        dynamic_cast<HistoryGlass &>(*get_ui_at(i)).save_data(file, data, websiteData);
        
        file << websiteData.get_end_data()
        << std::endl;
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

bool History::update()
{
    if (updateScroll())
    {
        for (int i = 0; i < get_num_of_elements(); i++)
        {
            HistoryGlass &glass = dynamic_cast<HistoryGlass &>(*get_ui_at(i));
            glass.set_pos(get_num_of_elements() - i - 1, scrollOffset);
        }
    }
    return (false);
}

void History::draw(void) const
{

    for (int i = 0; i < get_num_of_elements(); i++)
        dynamic_cast<HistoryGlass &>(*get_ui_at(i)).draww();
    DrawRectangleRounded(rect, ROUNDED, 10, UI::get_dcolor(UiColors::FIRST));

    BeginScissorMode(rect.x, rect.y, rect.width, rect.height);
    Win::draw();
    EndScissorMode();

    DrawRectangleRoundedLinesEx(rect, ROUNDED, 10, 6.0f, BLACK);
}