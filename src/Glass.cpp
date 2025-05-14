/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Glass.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kvanden- <kvanden-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/14 16:31:31 by kvanden-          #+#    #+#             */
/*   Updated: 2025/05/14 16:31:39 by kvanden-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Glass.hpp"

void draw_my_text(const char *name, float val, int x, int y)
{
    char buffer[64];
    snprintf(buffer, sizeof(buffer), name, val);
    DrawText(buffer, x, y, 30, DARKBLUE);
}

Glass::Glass(int ind, float height, GameData &data, std::function<void(UI &)> close_glas) : BufferedWin(100, height, 1550, 300),
                                                                                            ind(ind), data(data), close_glas_func(close_glas)
{
    rect = {10.0f, 10.0f, 1510, 80};
    add_ui(std::make_unique<Dropdown>(
        0, 10, 400, 50,
        data.names, "lol"));
    add_ui(std::make_unique<TextInp>(410, 10, 100, 50, [this](UI &ui)
                                     { save_druple(ui); }, "hoeveel?"));
    add_ui(std::make_unique<Button>(520, 10, 100, 50, "save",
                                    [this](UI &ui)
                                    { save_druple(ui); }));
    add_ui(std::make_unique<Button>(1400, 10, 100, 50, "save?",
                                    [this](UI &ui)
                                    { add_comment(ui); }));
    // add_ui(std::make_unique<Button>(1400, 10, 100, 50, "gek", close_glas));
    bar = StackedBar(630, 10, 400, 50);
    name = (Dropdown *)get_ui_at(0);
    amount = (TextInp *)get_ui_at(1);
}

void Glass::save_druple(UI &ui)
{
    int a;
    Element *elm;
    try
    {
        a = std::stoi(amount->get_text());
        elm = &data.get_element(name->get_selected_text());
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << '\n';
        return;
    }
    bar.add_value(elm->anion.ion, data.get_color(elm->anion.ion), elm->anion.mol * a);
    bar.add_value(elm->kation.ion, data.get_color(elm->kation.ion), elm->kation.mol * a);

    ph += elm->ph * a;
    mol += elm->mol * a;

    name->reset();
    amount->reset();
}

void Glass::add_comment(UI &ui)
{
    add_ui(std::make_unique<TextInp>(20, 80, 500, 50, close_glas_func, "coment?"));
    rect.height += 70;
}

void Glass::draww(void) const
{
    BeginTextureMode(this->win);
    clear();

    DrawRectangleRounded(rect, 0.2, 8, LIGHTGRAY);
    DrawRectangleRoundedLinesEx(rect, 0.2, 8, 6.0f, BLACK);
    draw_my_text("pH: %.2f", ph, 1050, 10);
    draw_my_text("mol: %.2f", mol, 1180, 10);
    bar.draw(this->get_mouse_pos());
    Win::draw();
    EndTextureMode();
}

void Glass::draw(void) const
{
    if (!_is_visible)
        return;
    Rectangle source = {0.0f, 0.0f, (float)this->win.texture.width, -(float)this->win.texture.height};
    Vector2 position = {this->pos.x, this->pos.y};
    DrawTextureRec(this->win.texture, source, position, WHITE);
}
