#include "TextInp.hpp"
#include "Mouse.hpp"
#include <iostream>

TextInp::TextInp(float x, float y, float w, float h, std::function<void(UI&)> callback, const std::string &tmp)
    : UI(callback), is_hover(false), bounds{x, y, w, h}, text(""), tmp(tmp)
    {

    }

void TextInp::update(void)
{
    is_hover = CheckCollisionPointRec(this->get_mouse_pos(), bounds);

    if (_is_locked)
    {
        if (is_hover)
            Mouse::update_cursor(MOUSE_CURSOR_NOT_ALLOWED);
        is_hover = false;
        return;
    }

    if (is_hover)
        Mouse::update_cursor(MOUSE_CURSOR_IBEAM);

    if (is_active && IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
        is_active = false;
    if ((is_hover && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) || (is_tabt && IsKeyPressed(KEY_ENTER) && !is_active))
    {
        is_active = !is_active;
        if (is_active && on_focus_clear)
            text.clear();
        return ;
    }
    if (is_active)
    {
        int key = GetCharPressed();
        while (key > 0)
        {
            if (key >= 32 && key <= 126)
                text += static_cast<char>(key);
            key = GetCharPressed();
        }
        if ((IsKeyPressed(KEY_BACKSPACE) || IsKeyPressedRepeat(KEY_BACKSPACE)) && !text.empty())
            text.pop_back();
        if (IsKeyPressed(KEY_ENTER))
            run_callback();
        if (IsKeyPressed(KEY_ESCAPE))
            is_active = false;
    }
}

bool TextInp::capture_tab(void)
{
    if (!_is_active)
        return (false);
    is_tabt = !is_tabt;
    if (!is_tabt)
        is_active = false;
    return (is_tabt);
}

void TextInp::draw(void) const
{
    // Achtergrond met afgeronde hoeken
    DrawRectangleRounded(bounds, 0.2f, 8, (is_hover || is_tabt) ? LIGHTGRAY : bg_color);

    // Border
    DrawRectangleRoundedLines(bounds, 0.2f, 8, DARKGRAY);

    const std::string &display_text = (text.empty() && !is_active) ? tmp : text;
    Color text_color = (text.empty() && !is_active) ? RED : BLACK;

    int font_size = 20;
    float max_width = bounds.width - 20;

    // ----------------------
    // 1. Tekst opdelen in regels (zonder tekenen)
    // ----------------------
    std::vector<std::string> lines;
    std::string line, word;

    for (size_t i = 0; i < display_text.length(); ++i)
    {
        char c = display_text[i];
        word += c;

        if (c == ' ' || c == '\t' || i == display_text.length() - 1)
        {
            std::string test_line = line + word;
            int test_width = MeasureText(test_line.c_str(), font_size);

            if (test_width > max_width && !line.empty())
            {
                lines.push_back(line);
                line = word;
            }
            else
            {
                line = test_line;
            }
            word.clear();
        }
    }
    if (!line.empty())
        lines.push_back(line);

    // ----------------------
    // 2. Verticale offset berekenen
    // ----------------------
    float total_height = lines.size() * (font_size + 2);
    float y = bounds.y + (bounds.height - total_height) / 2;

    // ----------------------
    // 3. Regels tekenen gecentreerd
    // ----------------------
    for (size_t i = 0; i < lines.size(); i++)
    {
        int line_width = MeasureText(lines[i].c_str(), font_size);
        float x = bounds.x + (bounds.width - line_width) / 2;

        DrawText(lines[i].c_str(), x, y, font_size, text_color);

        // Cursor op laatste regel
        if (is_active && i == lines.size() - 1 && (static_cast<int>(GetTime() * 2) % 2 == 0))
        {
            int cursor_x = x + line_width;
            DrawLine(cursor_x, y, cursor_x, y + font_size, BLACK);
        }

        y += font_size + 2;
    }
}

std::string &TextInp::get_text(void)
{
    return (text);
} 

const Rectangle &TextInp::get_rect(void)
{
    return (bounds);
}

void TextInp::move(int x, int y)
{
    bounds.x += x;
    bounds.y += y;
}

void TextInp::reset(void)
{
    text.clear();
}

void TextInp::remove_active(void)
{
    is_active = false;
    is_hover = false;
    is_tabt = false;
}

void TextInp::set_text(const std::string &text)
{
    this->text = text;
}

void TextInp::set_bg_color(const Color &color)
{
    bg_color = color;
}
