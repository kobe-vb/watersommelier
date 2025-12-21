#include "DropdownView.hpp"
#include "DropdownModel.hpp"
#include <raylib.h>
#include <algorithm>

#include "Mouse.hpp"

DropdownView::DropdownView(DropdownModel *model, float x, float y, float w, float h)
    : UIView(model), model(model), bounds{x, y, w, h}
{
}

void DropdownView::handle_text(void)
{

    bool change = false;
    int key = GetCharPressed();
    while (key > 0)
    {
        if (key >= 32 && key <= 126)
        {
            model->add_char((char)key);
            change = true;
        }
        key = GetCharPressed();
    }
    if ((IsKeyPressed(KEY_BACKSPACE) || IsKeyPressedRepeat(KEY_BACKSPACE)) && !model->is_empty())
    {
        change = true;
        model->remove_char();
    }
    else if (IsKeyPressed(KEY_DELETE))
    {
        model->reset();
        change = true;
    }
    if (change)
        model->on_change();
}

bool DropdownView::update()
{
    Vector2 mouse = this->get_mouse_pos();
    model->set_hover(CheckCollisionPointRec(mouse, bounds));

    if (model->is_locked())
    {
        if (model->is_hover())
            Mouse::update_cursor(MOUSE_CURSOR_NOT_ALLOWED);
        model->set_hover(false);
        return false;
    }

    if (model->is_hover())
        Mouse::update_cursor(MOUSE_CURSOR_IBEAM);

    if ((IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && model->is_hover()) ||
        ((IsKeyPressed(KEY_ENTER)) && is_tabt && !model->is_open()))
    {
        model->open();
        return false;
    }

    if (IsKeyPressed(KEY_ESCAPE))
        model->close();

    if (model->is_open())
    {
        handle_text();

        if (IsKeyPressed(KEY_UP) || IsKeyPressedRepeat(KEY_UP))
        {
            model->move_up();
        }
        else if (IsKeyPressed(KEY_DOWN) || IsKeyPressedRepeat(KEY_DOWN))
        {
            model->move_down();
        }
        else if (IsKeyPressed(KEY_ENTER))
        {
            model->select_current();
            clear_color(UiColors::BG);
        }

        model->move((int)(-GetMouseWheelMove()));

        for (int i = 0; i + model->get_index() < model->get_visible_size() && i < model->get_max_opt(); ++i)
        {
            Rectangle opt_rect = {bounds.x + 20, bounds.y + 30 + 20 + (i + 1) * (bounds.height - 30), bounds.width - 40, bounds.height - 40};
            if (CheckCollisionPointRec(mouse, opt_rect) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
            {
                model->select(i);
                clear_color(UiColors::BG);
                return false;
            }
        }
    }
    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
        model->close();

    return false;
}

void DropdownView::draw() const
{
    Vector2 mouse = this->get_mouse_pos();

    DrawRectangleRounded(bounds, 0.3f, 10, get_color(UiColors::SECOND));
    if (model->is_hover() || is_tabt)
        DrawRectangleRounded(bounds, 0.3f, 10, get_dcolor(UiColors::HOVER));

    const char *text;
    Color color;
    if (model->is_open() || !model->is_empty())
    {
        text = model->get_input_text().c_str();
        color = get_color(UiColors::TEXT);
    }
    else
    {
        text = model->get_placeholder().c_str();
        color = get_color(UiColors::PLACEHOLDER);
    }
    int textWidth = MeasureText(text, 20);

    float x = bounds.x + (bounds.width - textWidth) / 2;
    float y = bounds.y + (bounds.height - 20) / 2;

    DrawText(text, x, y, 20, color);

    if (model->is_open())
    {
        if ((static_cast<int>(GetTime() * 2) % 2 == 0)) // Blinking cursor
        {
            int text_width = MeasureText(model->get_input_text().c_str(), 20) + 3;
            DrawLineEx({x + text_width, y},
                       {x + text_width, y + 20}, 2, get_color(UiColors::TEXT));
        }

        Rectangle big_rect = {bounds.x, bounds.y + 10 + bounds.height, bounds.width, (bounds.height - 30) * model->get_max_opt() + 10};
        DrawRectangleRounded(big_rect, 0.1f, 10, get_color(UiColors::SECOND));
        bool is_hovering = CheckCollisionPointRec(mouse, big_rect);

        for (int i = 0; i + model->get_index() < model->get_visible_size() && i < model->get_max_opt(); ++i)
        {
            Rectangle opt_rect = {bounds.x + 20, bounds.y + 30 + 20 + (i + 1) * (bounds.height - 30), bounds.width - 40, bounds.height - 40};
            // DrawRectangleRounded(opt_rect, 0.3f, 10, get_color(UiColors::SECOND));
            if (CheckCollisionPointRec(mouse, opt_rect) || (!is_hovering && i == model->get_current_index()))
            {
                if (is_hovering)
                    Mouse::update_cursor(MOUSE_CURSOR_POINTING_HAND);
                DrawRectangleRounded(opt_rect, 0.3f, 10, get_dcolor(UiColors::HOVER));
            }
            DrawText(model->get_option(i).c_str(), opt_rect.x + 10, opt_rect.y + 10, 20, get_color(UiColors::TEXT));
        }
    }
}

bool DropdownView::capture_tab(int direction)
{
    UIView::capture_tab(direction);
    if (!is_tabt)
        model->close();
    return (is_tabt);
}
