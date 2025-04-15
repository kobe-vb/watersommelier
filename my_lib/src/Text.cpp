/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Text.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kvanden- <kvanden-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/24 15:18:16 by kvanden-          #+#    #+#             */
/*   Updated: 2025/03/24 18:54:57 by kvanden-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Text.hpp"
#include <iostream>

Text::Text(float x, float y, float w, float h, std::function<void(UI&)> callback)
    : UI(callback), is_hover(false), bounds{x, y, w, h}, text("") {}

void Text::update(void)
{
    is_hover = CheckCollisionPointRec(GetMousePosition(), bounds);
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
        if (IsKeyPressed(KEY_BACKSPACE) && !text.empty())
            text.pop_back();
        if (IsKeyPressed(KEY_ENTER))
            run_callback();
        if (IsKeyPressed(KEY_ESCAPE))
            is_active = false;
    }
}

bool Text::capture_tab(void)
{
    if (!_is_active)
        return (false);
    is_tabt = !is_tabt;
    if (!is_tabt)
        is_active = false;
    return (is_tabt);
}

void Text::draw(void) const
{
    DrawRectangleRec(bounds, (is_hover || is_tabt) ? LIGHTGRAY : GRAY);
    DrawText(text.c_str(), bounds.x + 10, bounds.y + 10, 20, BLACK);

    if (is_active && (static_cast<int>(GetTime() * 2) % 2 == 0))
    {
        int text_width = MeasureText(text.c_str(), 20);
        DrawLine(bounds.x + 10 + text_width, bounds.y + 10,
                 bounds.x + 10 + text_width, bounds.y + 30, BLACK);
    }
}

std::string &Text::get_text(void)
{
    return (text);
} 

const Rectangle &Text::get_rect(void)
{
    return (bounds);
}

void Text::move(int x, int y)
{
    bounds.x += x;
    bounds.y += y;
}
