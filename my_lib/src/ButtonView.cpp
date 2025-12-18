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

    DrawRectangleRounded(bounds, 0.3f, 10, get_color(UiColors::SECOND));
    if (model->is_hover() || is_tabt)
        DrawRectangleRounded(bounds, 0.3f, 10, get_dcolor(UiColors::HOVER));
    // DrawRectangleRoundedLinesEx(bounds, 0.3f, 10, 2.0f, get_color(UiColors::BORDER));
    
    int fontSize = 20;
    int textWidth = MeasureText(model->get_text().c_str(), fontSize);
    int textHeight = fontSize;

    float textX = bounds.x + (bounds.width - textWidth) / 2;
    float textY = bounds.y + (bounds.height - textHeight) / 2;

    DrawText(model->get_text().c_str(), textX, textY, fontSize, get_color(UiColors::TEXT));

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
