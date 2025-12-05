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
            Rectangle opt_rect = {bounds.x, bounds.y + (i + 1) * bounds.height, bounds.width, bounds.height};
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

    DrawRectangleRec(bounds, get_color(UiColors::BG));
    if (model->is_hover() || is_tabt)
        DrawRectangleRec(bounds, get_dcolor(UiColors::HOVER));
    DrawText(model->is_empty() ? model->get_placeholder().c_str() : model->get_input_text().c_str(), bounds.x + 10, bounds.y + 10, 20, model->is_empty() ? DARKGRAY : BLACK);

    if (model->is_open())
    {
        for (int i = 0; i + model->get_index() < model->get_visible_size() && i < model->get_max_opt(); ++i)
        {
            Rectangle opt_rect = {bounds.x, bounds.y + (i + 1) * bounds.height, bounds.width, bounds.height};
            DrawRectangleRec(opt_rect, CheckCollisionPointRec(mouse, opt_rect) || i == model->get_current_index() ? LIGHTGRAY : GRAY);
            DrawText(model->get_option(i).c_str(), opt_rect.x + 10, opt_rect.y + 10, 20, BLACK);
        }

        if ((static_cast<int>(GetTime() * 2) % 2 == 0))
        {
            int text_width = MeasureText(model->get_input_text().c_str(), 20);
            DrawLine(bounds.x + 10 + text_width, bounds.y + 10,
                     bounds.x + 10 + text_width, bounds.y + 30, BLACK);
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
