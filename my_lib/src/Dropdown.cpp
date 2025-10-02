#include "Dropdown.hpp"
#include <raylib.h>
#include <algorithm>

#include "Mouse.hpp"

Dropdown::Dropdown(float x, float y, float w, float h, const std::vector<std::string> &opts, const std::string &ph, std::function<void(UI &)> cb)
    : UI(cb), bounds{x, y, w, h}, options(opts), is_open(false), placeholder(ph)
{
    input_text = "";
    filtered = options;
}

void Dropdown::filter_options()
{
    filtered.clear();
    for (const auto &opt : options)
    {
        if (opt.find(input_text) != std::string::npos)
            filtered.push_back(opt);
    }
}

void Dropdown::handle_text(void)
{
    bool change = false;
    int key = GetCharPressed();
    while (key > 0)
    {
        if (key >= 32 && key <= 126)
        {
            input_text += static_cast<char>(key);
            change = true;
        }
        key = GetCharPressed();
    }
    if ((IsKeyPressed(KEY_BACKSPACE) || IsKeyPressedRepeat(KEY_BACKSPACE)) && !input_text.empty())
    {
        change = true;
        input_text.pop_back();
    }
    else if (IsKeyPressed(KEY_DELETE))
    {
        input_text.clear();
        change = true;
    }
    if (change)
    {
        filter_options();
        ind = 0;
        current_ind = 0;
    }
}
void Dropdown::update()
{
    Vector2 mouse = this->get_mouse_pos();
    hover = CheckCollisionPointRec(mouse, bounds);

    if (_is_locked)
    {
        if (hover)
            Mouse::update_cursor(MOUSE_CURSOR_NOT_ALLOWED);
        hover = false;
        return;
    }

    if (hover)
        Mouse::update_cursor(MOUSE_CURSOR_IBEAM);

    if ((IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && hover) ||
        ((IsKeyPressed(KEY_ENTER)) && is_tabt && !is_open))
    {
        is_open = !is_open;
        filter_options();
        ind = 0;
        current_ind = 0;
        return;
    }

    if (IsKeyPressed(KEY_ESCAPE))
        is_open = false;

    if (is_open)
    {
        handle_text();

        if (IsKeyPressed(KEY_UP) || IsKeyPressedRepeat(KEY_UP))
        {
            if (current_ind == 0)
                ind--;
            else
                current_ind--;
        }
        else if (IsKeyPressed(KEY_DOWN) || IsKeyPressedRepeat(KEY_DOWN))
        {
            if (current_ind == (max_opt - 1))
                ind++;
            else
                current_ind++;
        }
        else if (IsKeyPressed(KEY_ENTER))
        {
            if (filtered.size() == 1)
            {
                is_open = false;
                run_callback();
            }
            input_text = filtered[current_ind + ind];
            filter_options();
            ind = 0;
            current_ind = 0;
        }

        ind -= (int)(GetMouseWheelMove());
        ind = std::clamp(ind, 0, std::max(0, (int)filtered.size() - 1 - max_opt));

        for (int i = 0; i + ind < (int)filtered.size() && i < max_opt; ++i)
        {
            Rectangle opt_rect = {bounds.x, bounds.y + (i + 1) * bounds.height, bounds.width, bounds.height};
            if (CheckCollisionPointRec(mouse, opt_rect) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
            {
                input_text = filtered[i + ind];
                filter_options();
                is_open = false;
                run_callback();
                break;
            }
        }
    }
}

void Dropdown::draw() const
{
    Vector2 mouse = this->get_mouse_pos();

    DrawRectangleRec(bounds, hover || is_tabt ? LIGHTGRAY : GRAY);
    DrawText(input_text.empty() ? placeholder.c_str() : input_text.c_str(), bounds.x + 10, bounds.y + 10, 20, input_text.empty() ? DARKGRAY : BLACK);

    if (is_open)
    {
        for (int i = 0; i + ind < (int)filtered.size() && i < max_opt; ++i)
        {
            Rectangle opt_rect = {bounds.x, bounds.y + (i + 1) * bounds.height, bounds.width, bounds.height};
            DrawRectangleRec(opt_rect, CheckCollisionPointRec(mouse, opt_rect) || i == current_ind ? LIGHTGRAY : GRAY);
            DrawText(filtered[i + ind].c_str(), opt_rect.x + 10, opt_rect.y + 10, 20, BLACK);
        }

        if ((static_cast<int>(GetTime() * 2) % 2 == 0))
        {
            int text_width = MeasureText(input_text.c_str(), 20);
            DrawLine(bounds.x + 10 + text_width, bounds.y + 10,
                     bounds.x + 10 + text_width, bounds.y + 30, BLACK);
        }
    }
}

std::string Dropdown::get_selected_text() const
{
    return input_text;
}

int Dropdown::get_selected_ind() const
{
    // todo
    return (0);
}

void Dropdown::set_max_opt(int i)
{
    max_opt = i;
}

bool Dropdown::capture_tab(void)
{
    if (!_is_active)
        return (false);
    is_tabt = !is_tabt;
    if (!is_tabt)
        is_open = false;
    return (is_tabt);
}

void Dropdown::reset(void)
{
    input_text.clear();
    ind = 0;
    current_ind = 0;
}

void Dropdown::set(int index)
{
    input_text = options[index];
    ind = 0;
    current_ind = 0;
}