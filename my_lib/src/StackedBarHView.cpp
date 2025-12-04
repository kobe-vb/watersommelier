#include "StackedBarModel.hpp"
#include "StackedBarHView.hpp"

StackedBarHView::StackedBarHView(StackedBarModel *model, int x, int y, int w, int h)
    : model(model), x(x), y(y), width(w), height(h) {}

StackedBarHView::StackedBarHView(const StackedBarHView &other)
    : model(other.model),
      x(other.x), y(other.y), width(other.width), height(other.height)
{
}

StackedBarHView &StackedBarHView::operator=(const StackedBarHView &other)
{
    if (this != &other)
    {
        model = other.model;
        x = other.x;
        y = other.y;
        width = other.width;
        height = other.height;
    }
    return *this;
}

void StackedBarHView::set_pos(int x, int y)
{
    this->x = x;
    this->y = y;
}

void StackedBarHView::draw(Vector2 mouse) const
{
    if (model->get_total_volume() <= 0)
    {
        DrawRectangleLinesEx({x, y, width, height}, 3.0f, DARKGRAY);
        return;
    }
    float currentX = static_cast<float>(x);
    bool is_hover = false;
    std::string label;

    for (const auto &segment : model->get_data())
    {
        float segmentWidth = (segment.val / model->get_total_volume()) * width;
        Rectangle rect = {currentX, static_cast<float>(y), segmentWidth, static_cast<float>(height)};

        DrawRectangle(static_cast<int>(currentX), y, static_cast<int>(segmentWidth), height, segment.col);

        if (CheckCollisionPointRec(mouse, rect))
        {
            label = segment.name + " (" + std::to_string(static_cast<int>((segment.val / model->get_total_volume()) * 100)) + "%)";
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
