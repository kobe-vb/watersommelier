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

#include "TokelView.hpp"
#include "Mouse.hpp"

TokelView::TokelView(TokelModel *model, float x, float y, float w, float h)
    : UIView(model), model(model), bounds({x, y, w, h})
{
    int fontSize = 20;
    float padding = 10.0f;
    float maxWidth = bounds.width - 2 * padding;

    display_text = model->get_text();
    while (!display_text.empty() && MeasureText((display_text + "...").c_str(), fontSize) > maxWidth) {
        display_text.pop_back();
    }
    if (display_text != model->get_text())
        display_text += "...";

    int displayWidth = MeasureText(display_text.c_str(), fontSize);
    textX = bounds.x + (bounds.width - displayWidth) / 2.0f;
    textY = bounds.y + (bounds.height - fontSize) / 2.0f;

}

bool TokelView::update(void)
{
    model->set_hover(CheckCollisionPointRec(this->get_mouse_pos(), bounds));
    if (model->is_hover())
        Mouse::update_cursor(MOUSE_CURSOR_POINTING_HAND);

    if (model->is_hover() && (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) || IsKeyPressed(KEY_ENTER)))
        model->tokel();
    else if (IsKeyPressed(KEY_ENTER) && is_tabt)
        model->tokel();
    else
        return false;
    if (model->is_active())
        model->run_callback();
    return (false);
}

void TokelView::draw(void) const
{
    int fontSize = 20;

    Color baseColor = model->is_active() ? get_color(UiColors::ON) : get_color(UiColors::OFF); 
    DrawRectangleRounded(bounds, 0.2f, 10, baseColor);
    if (model->is_hover() || is_tabt)
        DrawRectangleRounded(bounds, 0.2f, 10, get_dcolor(UiColors::HOVER));
    DrawRectangleRoundedLinesEx(bounds, 0.2f, 10, 2, get_dcolor(UiColors::BORDER));

    DrawText(display_text.c_str(), (int)textX, (int)textY, fontSize, WHITE);
}





