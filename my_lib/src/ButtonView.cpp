/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ButtonView.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kvanden- <kvanden-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/24 14:48:43 by kvanden-          #+#    #+#             */
/*   Updated: 2025/05/13 16:55:44 by kvanden-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ButtonView.hpp"
#include "ButtonModel.hpp"
#include "raylib.h"
#include "Mouse.hpp"

ButtonView::ButtonView(ButtonModel *model, float x, float y, float w, float h, bool center)
    : UIView(model), model(model), bounds{x, y, w, h}
{
    if (center)
        bounds.x -= MeasureText(model->get_text().c_str(), 20) / 2;
}

void ButtonView::draw(void) const
{
    // Kleur afhankelijk van hover/focus
    Color btnColor = (model->is_hover() || is_tabt) ? LIGHTGRAY : GRAY;

    // Teken afgeronde rechthoek (0.3f = afrondingsgraad, 10 = segments)
    DrawRectangleRounded(bounds, 0.3f, 10, btnColor);

    // Tekst uitmeten
    int fontSize = 20;
    int textWidth = MeasureText(model->get_text().c_str(), fontSize);
    int textHeight = fontSize; // Raylib gebruikt fontSize als "hoogte"

    // Gecentreerde positie berekenen
    float textX = bounds.x + (bounds.width - textWidth) / 2;
    float textY = bounds.y + (bounds.height - textHeight) / 2;

    // Tekst tekenen
    DrawText(model->get_text().c_str(), textX, textY, fontSize, BLACK);

    // Optioneel: rand tekenen (outline)
    DrawRectangleRoundedLinesEx(bounds, 0.3f, 10, 4.0f, BLACK);
}

bool ButtonView::update(void)
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
        Mouse::update_cursor(MOUSE_CURSOR_POINTING_HAND);
    if ((model->is_hover() && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) || (is_tabt && IsKeyPressed(KEY_ENTER)))
        model->run_callback();
    return (false);
}
