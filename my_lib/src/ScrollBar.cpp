/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ScrollBar.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kvanden- <kvanden-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/13 17:26:40 by kvanden-          #+#    #+#             */
/*   Updated: 2025/05/13 18:27:14 by kvanden-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ScrollBar.hpp"

ScrollBar::ScrollBar(Rectangle bouns, float winheight, float scrollheight, std::function<void(float)> callback)
{
    this->bounds = bouns;
    this->winheight = winheight;
    this->scrollheight = scrollheight;
    this->callback = callback;

    this->scroll_bar = {
        this->bounds.x + 5,
        this->bounds.y + 5,
        this->bounds.width - 10,
        0
    };

    scrollpos = scrollheight;
    add_height(0);
}

void ScrollBar::add_height(float height)
{
    this->scrollheight += height;
    this->scroll_update();
}

float ScrollBar::get_height()
{
    return (scrollpos);
}

void ScrollBar::scroll_update(void)
{
    if (scrollheight <= winheight)
    {
        scroll_bar.height = bounds.height - 10;
        scroll_bar.y = bounds.y + 5;
    }
    else
    {
        float ratio = winheight / scrollheight;
        scroll_bar.height = std::max((bounds.height - 10) * ratio, 10.0f);

        float scrollRange = scrollheight - winheight;
        float pixelRange = bounds.height - 10 - scroll_bar.height;

        float scrolled = scrollheight - scrollpos - winheight;
        scroll_bar.y = bounds.y + 5 + (scrolled / scrollRange) * pixelRange;
    }
}

void ScrollBar::scroll(float height)
{
    if (scrollheight <= winheight)
        return;

    float oldScroll = scrollpos;

    scrollpos += height;
    scrollpos = std::min(scrollpos, scrollheight);
    scrollpos = std::max(scrollpos, winheight);

    if (callback && scrollpos != oldScroll)
        callback(scrollpos - oldScroll);

    scroll_update();
}

void ScrollBar::update(void)
{
    if (IsMouseOver() && GetMouseWheelMove() != 0)
    {
        scroll(-GetMouseWheelMove() * 20);
    }
}

bool ScrollBar::IsMouseOver() const
{
    Vector2 mouse = GetMousePosition();
    return CheckCollisionPointRec(mouse, bounds);
} 


void ScrollBar::draw(void) const
{
    DrawRectangleRec(this->bounds, GRAY);
    DrawRectangleRec(this->scroll_bar, LIGHTGRAY);
}