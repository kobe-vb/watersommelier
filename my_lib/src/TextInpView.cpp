#include "TextInpView.hpp"
#include "TextInpModel.hpp"
#include "Mouse.hpp"
#include <iostream>

TextInpView::TextInpView(TextInpModel *model, float x, float y, float w, float h)
    : UIView(model), model(model), bounds{x, y, w, h}
{
}

bool TextInpView::update(void)
{
    model->set_hover(CheckCollisionPointRec(this->get_mouse_pos(), bounds));

    if (model->is_locked())
    {
        if (model->is_hover())
            Mouse::update_cursor(MOUSE_CURSOR_NOT_ALLOWED);
        model->set_hover(false);
        return false;
    }

    if (model->is_hover())
        Mouse::update_cursor(MOUSE_CURSOR_IBEAM);

    if (model->is_active() && IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
        set_active(false);
    if ((model->is_hover() && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) || (is_tabt && IsKeyPressed(KEY_ENTER) && !model->is_active()))
    {
        model->focus();
        return true;
    }
    if (model->is_active())
    {
        int key = GetCharPressed();
        while (key > 0)
        {
            model->add_char(static_cast<char>(key));
            key = GetCharPressed();
        }
        if ((IsKeyPressed(KEY_BACKSPACE) || IsKeyPressedRepeat(KEY_BACKSPACE)) && !model->is_empty())
            model->backspace();
        if (IsKeyPressed(KEY_ENTER))
            model->run_callback();
        if (IsKeyPressed(KEY_ESCAPE))
            set_active(false);
        // CTRL+V voor plakken
        if ((IsKeyDown(KEY_LEFT_CONTROL) || IsKeyDown(KEY_RIGHT_CONTROL)) && IsKeyPressed(KEY_V))
        {
            const char *clip = GetClipboardText();
            if (clip)
                model->add_text(clip);
        }
    }
    return (false);
}

void TextInpView::set_active(bool value)
{
    // UIView::set_active(value);
    model->set_active(value);
}

bool TextInpView::capture_tab(int direction)
{
    UIView::capture_tab(direction);
    if (!is_tabt)
        model->set_active(false);
    else
        model->set_active(true);
    return (is_tabt);
}

void TextInpView::draw(void) const
{

    // std::cout << "TextInpView: " << bounds.x << " " << bounds.y << " " << bounds.width << " " << bounds.height << std::endl;

    DrawRectangleRounded(bounds, 0.2f, 8, get_color(UiColors::BG));
    if (model->is_hover() || is_tabt)
        DrawRectangleRounded(bounds, 0.2f, 8, get_dcolor(UiColors::HOVER));

    // Border
    DrawRectangleRoundedLines(bounds, 0.2f, 8, DARKGRAY);

    const std::string display_text = model->get_display_text();
    Color text_color = (model->is_empty() && !model->is_active()) ? GRAY : BLACK;

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
        if (model->is_active() && i == lines.size() - 1 && (static_cast<int>(GetTime() * 2) % 2 == 0))
        {
            int cursor_x = x + line_width;
            DrawLine(cursor_x, y, cursor_x, y + font_size, BLACK);
        }

        y += font_size + 2;
    }
    if (model->is_active() && lines.size() == 0 && (static_cast<int>(GetTime() * 2) % 2 == 0))
    {
        int x = bounds.x + (bounds.width - font_size) / 2;
        int y = bounds.y + (bounds.height - font_size) / 2;
        DrawLine(x, y, x, y + font_size, BLACK);
    }
}

const Rectangle &TextInpView::get_rect(void)
{
    return (bounds);
}

void TextInpView::move(int x, int y)
{
    bounds.x += x;
    bounds.y += y;
}
