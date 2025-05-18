#include "StackedBar.hpp"

StackedBar::StackedBar(int x, int y, int w, int h)
    : x(x), y(y), width(w), height(h) {}

StackedBar::StackedBar(const StackedBar &other)
    : data(other.data), total_volume(other.total_volume),
      x(other.x), y(other.y), width(other.width), height(other.height)
{
}

StackedBar &StackedBar::operator=(const StackedBar &other)
{
    if (this != &other)
    {
        data = other.data;
        total_volume = other.total_volume;
        x = other.x;
        y = other.y;
        width = other.width;
        height = other.height;
    }
    return *this;
}

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

void StackedBar::draw(Vector2 mouse) const
{
    if (total_volume <= 0)
    {
        DrawRectangleLinesEx({x, y, width, height}, 3.0f, DARKGRAY);
        return;
    }
    float currentX = static_cast<float>(x);
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
        DrawRectangle(mouse.x + 10, mouse.y - 20, textWidth + 20, 30, BLACK);
        DrawText(label.c_str(), mouse.x + 20, mouse.y - 18, 20, WHITE);
    }
}
