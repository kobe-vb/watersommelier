#include "StackedBar.hpp"

StackedBar::StackedBar(int x, int y, int w, int h)
    : x(x), y(y), width(w), height(h) {}

void StackedBar::add_value(const std::string &name, Color col, float val)
{
    for (auto &segment : data)
    {
        if (segment.name == name)
        {
            segment.val += val;
            total_volume += val;
            return;
        }
    }
    data.push_back({name, col, val});
    total_volume += val;
}

void StackedBar::draw() const
{
    if (total_volume <= 0)
    {
        DrawRectangleLinesEx({x, y, width, height}, 3.0f, DARKGRAY);
        return;
    }
    float currentX = static_cast<float>(x);
    Vector2 mouse = GetMousePosition();
    bool is_hover = false;
    std::string label;

    for (const auto &segment : data)
    {
        float segmentWidth = (segment.val / total_volume) * width;
        Rectangle rect = {currentX, static_cast<float>(y), segmentWidth, static_cast<float>(height)};

        DrawRectangle(static_cast<int>(currentX), y, static_cast<int>(segmentWidth), height, segment.col);

        if (CheckCollisionPointRec(mouse, rect))
        {
            label = segment.name + " (" + std::to_string(static_cast<int>((segment.val / total_volume) * 100)) + "%)";
            is_hover = true;
            DrawRectangleLinesEx({currentX, y, segmentWidth, height}, 4, BLACK);
        }

        currentX += segmentWidth;
    }
    DrawRectangleLinesEx({x, y, width, height}, 3.0f, DARKGRAY);
    if (is_hover)
    {
        int textWidth = MeasureText(label.c_str(), 20) + 10;
        DrawRectangle(mouse.x + 10, mouse.y - 20, textWidth, 20, BLACK);
        DrawText(label.c_str(), mouse.x + 15, mouse.y - 18, 20, WHITE);
    }
}
