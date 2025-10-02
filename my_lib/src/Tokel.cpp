/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Tokel.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kvanden- <kvanden-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/24 12:22:43 by kvanden-          #+#    #+#             */
/*   Updated: 2025/05/13 16:56:01 by kvanden-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Tokel.hpp"
#include "Mouse.hpp"

Tokel::Tokel(float x, float y, float w, float h, const std::string &text, std::function<void(UI&)> callback)
    : UI(callback), active(false), is_hover(false), bounds({x, y, w, h})
{
    int fontSize = 20;
    float padding = 10.0f;
    float maxWidth = bounds.width - 2 * padding;

    this->text = text;
    display_text = text;
    while (!display_text.empty() && MeasureText((display_text + "...").c_str(), fontSize) > maxWidth) {
        display_text.pop_back();
    }
    if (display_text != text)
        display_text += "...";

    int displayWidth = MeasureText(display_text.c_str(), fontSize);
    textX = bounds.x + (bounds.width - displayWidth) / 2.0f;
    textY = bounds.y + (bounds.height - fontSize) / 2.0f;

}

Tokel::~Tokel(void) {}

void Tokel::update(void)
{
    is_hover = CheckCollisionPointRec(this->get_mouse_pos(), bounds);
    if (is_hover)
        Mouse::update_cursor(MOUSE_CURSOR_POINTING_HAND);

    if (is_hover && (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) || IsKeyPressed(KEY_ENTER)))
        active = !active;
    else if (IsKeyPressed(KEY_ENTER) && is_tabt)
        active = !active;
    else
        return ;
    if (active)
        run_callback();
}

void Tokel::draw(void) const
{
    int fontSize = 20;

    Color baseColor = active ? get_color(UiColors::ON) : get_color(UiColors::OFF); 

    DrawRectangleRounded(bounds, 0.2f, 10, baseColor);
    if (is_hover || is_tabt)
        DrawRectangleRounded(bounds, 0.2f, 10, get_dcolor(UiColors::HOVER));
    DrawRectangleRoundedLinesEx(bounds, 0.2f, 10, 2, get_dcolor(UiColors::BORDER));

    DrawText(display_text.c_str(), (int)textX, (int)textY, fontSize, WHITE);
}



void Tokel::set_tokel(bool value)
{
    active = value;
    if (value)
        run_callback();
}

const std::string &Tokel::get_text(void)
{
    return (text);
} 