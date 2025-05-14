/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   BufferedWin.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kvanden- <kvanden-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/13 16:02:59 by kvanden-          #+#    #+#             */
/*   Updated: 2025/05/14 16:08:46 by kvanden-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "BufferedWin.hpp"

BufferedWin::BufferedWin(float x, float y, float w, float h) : Win(), pos({x, y})
{
    this->win = LoadRenderTexture(w, h);
}

BufferedWin::~BufferedWin()
{
    UnloadRenderTexture(this->win);
}

void BufferedWin::add_ui(std::unique_ptr<UI> element)
{
    element->set_parent(this);
    Win::add_ui(std::move(element));
}

void BufferedWin::clear(void) const
{
    ClearBackground(BLANK);
}

void BufferedWin::draw() const
{
    if (!_is_visible)
        return;
    BeginTextureMode(this->win);
    clear();
    Win::draw();
    EndTextureMode();
    DrawTexture(this->win.texture, this->pos.x, this->pos.y, WHITE);
}

Vector2 BufferedWin::get_mouse_pos(void) const
{
    return (Vector2{GetMousePosition().x - this->pos.x, GetMousePosition().y - this->pos.y});
}

void BufferedWin::move(int x, int y)
{
    this->pos.x += x;
    this->pos.y += y;
}

void BufferedWin::set_pos(int x, int y)
{
    this->pos.x = x;
    this->pos.y = y;
}

Vector2 BufferedWin::get_pos(void) const
{
    return (this->pos);
}