/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Button.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kvanden- <kvanden-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/24 14:48:43 by kvanden-          #+#    #+#             */
/*   Updated: 2025/05/13 16:55:44 by kvanden-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Button.hpp"
#include "raylib.h"
#include "Mouse.hpp"

Button::Button(float x, float y, float w, float h, const std::string &text, std::function<void(UI &)> callback)
    : UI(callback), is_hover(false), bounds{x, y, w, h}, text(text) {}

Button::~Button(void) {}

void Button::draw(void) const
{
    // Kleur afhankelijk van hover/focus
    Color btnColor = (is_hover || is_tabt) ? LIGHTGRAY : GRAY;

    // Teken afgeronde rechthoek (0.3f = afrondingsgraad, 10 = segments)
    DrawRectangleRounded(bounds, 0.3f, 10, btnColor);

    // Tekst uitmeten
    int fontSize = 20;
    int textWidth = MeasureText(text.c_str(), fontSize);
    int textHeight = fontSize; // Raylib gebruikt fontSize als "hoogte"

    // Gecentreerde positie berekenen
    float textX = bounds.x + (bounds.width - textWidth) / 2;
    float textY = bounds.y + (bounds.height - textHeight) / 2;

    // Tekst tekenen
    DrawText(text.c_str(), textX, textY, fontSize, BLACK);

    // Optioneel: rand tekenen (outline)
    DrawRectangleRoundedLinesEx(bounds, 0.3f, 10, 4.0f, BLACK);
}

void Button::update(void)
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
        Mouse::update_cursor(MOUSE_CURSOR_POINTING_HAND);
    if ((is_hover && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) || (is_tabt && IsKeyPressed(KEY_ENTER)))
        run_callback();
}

void Button::set_text(const std::string &text)
{
    this->text = text;
}