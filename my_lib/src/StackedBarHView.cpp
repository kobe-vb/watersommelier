#include "StackedBarModel.hpp"
#include "StackedBarHView.hpp"

#include <sstream>
#include <iomanip>

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

void StackedBarHView::set_converter(std::function<double(double)> fn, const std::string &u)
{
    value_converter = fn;
    unit = u;
}

void StackedBarHView::set_pos(int x, int y)
{
    this->x = x;
    this->y = y;
}

void StackedBarHView::update(Vector2 mouse)
{
    Rectangle fullBar = {(float)x, (float)y, (float)width, (float)height};
    bool bar_hovered = CheckCollisionPointRec(mouse, fullBar);

    if (bar_hovered && IsKeyPressed(KEY_I))
        show_info = !show_info;
    if (!bar_hovered)
        show_info = false;
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
            double pct = (segment.val / model->get_total_volume()) * 100.0;
            std::ostringstream ss;
            ss << segment.name << " (" << std::fixed << std::setprecision(2) << pct << "%)";
            label = ss.str();
            is_hover = true;
            DrawRectangleLinesEx({currentX, y, segmentWidth, height}, 4, BLACK);
        }

        currentX += segmentWidth;
    }
    DrawRectangleLinesEx({x, y, width, height}, 3.0f, DARKGRAY);

    if (show_info)
    {
        int rows = model->get_data().size() + 1; // +1 voor totaal
        int tableX = x + width + 20;
        int tableY = y;
        int rowH = 28, colW = 290;

        DrawRectangle(tableX, tableY, colW, rows * rowH + 10, Fade(BLACK, 0.85f));

        int row = 0;
        for (const auto &seg : model->get_data())
        {
            double pct = (seg.val / model->get_total_volume()) * 100.0;
            double converted = value_converter(seg.val);

            std::ostringstream ss;
            ss << seg.name << ": " << std::fixed << std::setprecision(2)
               << converted << " " << unit
               << "  (" << pct << "%)";

            DrawRectangle(tableX + 5, tableY + 5 + row * rowH, 15, 15, seg.col);
            DrawText(ss.str().c_str(), tableX + 25, tableY + 5 + row * rowH, 18, WHITE);
            row++;
        }

        double total_converted = value_converter(model->get_total_volume());
        std::ostringstream tot;
        tot << "Totaal: " << std::fixed << std::setprecision(2) << total_converted << " " << unit;
        DrawText(tot.str().c_str(), tableX + 5, tableY + 5 + row * rowH, 18, YELLOW);
    }

    if (is_hover)
    {
        int textWidth = MeasureText(label.c_str(), 20) + 10;
        DrawRectangle(mouse.x + 10, mouse.y - 20, textWidth + 20, 30, BLACK);
        DrawText(label.c_str(), mouse.x + 20, mouse.y - 18, 20, WHITE);
    }
}
