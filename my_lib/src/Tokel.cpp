/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Tokel.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kvanden- <kvanden-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/24 12:22:43 by kvanden-          #+#    #+#             */
/*   Updated: 2025/03/24 17:03:40 by kvanden-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Tokel.hpp"

Tokel::Tokel(float x, float y, float w, float h, const std::string &text, std::function<void(UI&)> callback)
    : UI(callback), active(false), is_hover(false), bounds({x, y, w, h}), text(text)
{
}

Tokel::~Tokel(void) {}

void Tokel::update(void)
{
    is_hover = CheckCollisionPointRec(GetMousePosition(), bounds);
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
    Color col = {255, 0, 0, 255};
    if (active)
        col = {0, 255, 0, 255};
    if (is_hover)
        col.a = 30;
    if (is_tabt)
        col.a = 30;
        
    DrawRectangleRec(bounds, col);
    DrawText(text.c_str(), bounds.x, bounds.y, 20, {255, 255, 255, 255});
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