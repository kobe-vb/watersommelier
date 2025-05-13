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

Button::Button(float x, float y, float w, float h, const std::string &text, std::function<void(UI&)> callback)
: UI(callback), is_hover(false), bounds{ x, y, w, h }, text(text){}

    Button::~Button(void) {}

void Button::draw(void) const {
    DrawRectangleRec(bounds, (is_hover || is_tabt) ? LIGHTGRAY : GRAY);
    DrawText(text.c_str(), bounds.x + 10, bounds.y + 10, 20, BLACK);
}

void Button::update(void) {
    is_hover = CheckCollisionPointRec(this->get_mouse_pos(), bounds);

    if ((is_hover && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) || (is_tabt && IsKeyPressed(KEY_ENTER)))
        run_callback();
}
