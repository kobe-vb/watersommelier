#include "TextInp.hpp"
#include <iostream>

TextInp::TextInp(float x, float y, float w, float h, std::function<void(UI&)> callback, const std::string &tmp)
    : UI(callback), is_hover(false), bounds{x, y, w, h}, text(""), tmp(tmp)
    {

    }

void TextInp::update(void)
{
    is_hover = CheckCollisionPointRec(this->get_mouse_pos(), bounds);
    if (is_active && IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
        is_active = false;
    if ((is_hover && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) || (is_tabt && IsKeyPressed(KEY_ENTER) && !is_active))
    {
        is_active = !is_active;
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
    DrawRectangleRec(bounds, (is_hover || is_tabt) ? LIGHTGRAY : bg_color);

    const std::string &display_text = (text.empty() && !is_active) ? tmp : text;
    Color text_color = (text.empty() && !is_active) ? RED : BLACK;

    float x = bounds.x + 10;
    float y = bounds.y + 10;
    float max_width = bounds.width - 20;
    int font_size = 20;

    std::string line;
    std::string word;

    for (size_t i = 0; i < display_text.length(); ++i)
    {
        char c = display_text[i];
        word += c;

        // Check of dit het einde van een "woord" is (ruimte, tab of einde string)
        if (c == ' ' || c == '\t' || i == display_text.length() - 1)
        {
            std::string test_line = line + word;
            int test_width = MeasureText(test_line.c_str(), font_size);

            if (test_width > max_width && !line.empty())
            {
                // Huidige regel past niet met extra woord, dus eerst de lijn tekenen
                DrawText(line.c_str(), x, y, font_size, text_color);
                y += font_size + 2;
                line = word; // Nieuwe regel begint met huidig woord
            }
            else
            {
                line = test_line; // Voeg toe aan huidige regel
            }

            word.clear(); // Klaar met woord
        }
    }

    // Laatste regel tekenen (als iets overblijft)
    if (!line.empty())
        DrawText(line.c_str(), x, y, font_size, text_color);

    // Cursor (blinkt als actief)
    if (is_active && (static_cast<int>(GetTime() * 2) % 2 == 0))
    {
        int cursor_x = x + MeasureText(line.c_str(), font_size);
        DrawLine(cursor_x, y, cursor_x, y + font_size, BLACK);
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
