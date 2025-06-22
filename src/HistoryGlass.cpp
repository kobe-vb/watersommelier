/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HistoryGlass.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kvanden- <kvanden-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/19 11:37:33 by kvanden-          #+#    #+#             */
/*   Updated: 2025/05/19 12:49:13 by kvanden-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "HistoryGlass.hpp"
#include "Settings.hpp"

void draw_my_text(const char *name, float val, int x, int y)
{
    char buffer[64];
    snprintf(buffer, sizeof(buffer), name, val);
    DrawText(buffer, x, y, 30, DARKBLUE);
}

HistoryGlass::HistoryGlass(int i, Glass &glass) : 
                                                  BufferedWin((GetScreenWidth() * 1 / 3) + PEDING, LINE + PEDING * 4,
                                                              (GetScreenWidth() * 1 / 3) - PEDING * 4, 120),
                                                  i(i),
                                                  bar(glass.bar),
                                                  comment(glass.get_comment()),
                                                  ph(glass.ph), mol(glass.mol)
{
    bar.set_pos(0, 0);
    rect = {((float)GetScreenWidth() * 1 / 3) + PEDING, LINE + PEDING * 4, (float)win.texture.width ,(float)win.texture.height};
};

void HistoryGlass::save_data(std::ofstream &file)
{
    file << mol << "," << ph << "," << comment << "," << keyWords << std::endl;
    // TODO: save elements
}

void HistoryGlass::set_pos(int i, float scrollOffset)
{
    this->pos.x = (GetScreenWidth() * 1 / 3) + PEDING;
    this->pos.y = LINE + PEDING * 4 + (i * 140) - scrollOffset;

    rect.x = pos.x;
    rect.y = pos.y;

    float screenTop = LINE;
    float screenBottom = GetScreenHeight() - PEDING;
    
    if (rect.y + rect.height < screenTop || rect.y > screenBottom)
    {
        this->_is_visible = false;
    }
    else
    {
        this->_is_visible = true;
    }

    this->i++; // compiler
}

void HistoryGlass::draw(void) const
{
    if (!_is_visible)
        return;
        
    DrawRectangleRounded(rect, ROUNDED, 10, COL_1);
    DrawRectangleRoundedLinesEx(rect, ROUNDED, 10, 6.0f, BLACK);

    
        DrawTextureRec(
        this->win.texture,
        (Rectangle){0, 0, (float)this->win.texture.width, -(float)this->win.texture.height},
        this->pos,
        WHITE
    );
};

void HistoryGlass::draww(void) const
{
    if (!_is_visible)
        return;

    BeginTextureMode(this->win);
    clear();
    
    draw_my_text("pH: %.2f", ph, PEDING * 3, 80);
    draw_my_text("mol: %.2f, ####### #kobe #cool #8/10", mol, PEDING * 3 + 200, 80);
    bar.draw(this->get_mouse_pos());
    EndTextureMode();    
};