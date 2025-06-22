/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Glass.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kvanden- <kvanden-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/14 16:31:31 by kvanden-          #+#    #+#             */
/*   Updated: 2025/05/20 20:33:55 by kvanden-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Glass.hpp"
#include "Settings.hpp"

static void draw_my_text(const char *name, float val, int x, int y)
{
    char buffer[64];
    snprintf(buffer, sizeof(buffer), name, val);
    DrawText(buffer, x, y, 30, DARKBLUE);
}

Glass::Glass(GameData &data, std::function<void(UI &)> close_glas, Sim &sim) : 
data(data), sim(sim), next_glas_func(close_glas)
{
    rect.height = GetScreenHeight() - LINE - PEDING;
    rect.width = (GetScreenWidth() * 1 / 3) - (PEDING * 3);
    rect.x = PEDING;
    rect.y = LINE;

    bar = StackedBar(PEDING * 2, LINE + PEDING * 4, rect.width - PEDING * 2, 60);

    add_ui(std::make_unique<Dropdown>(
        PEDING * 2, LINE + 200,
        rect.width - 200, 60,
        data.names, "lol"));

    add_ui(std::make_unique<TextInp>(
        PEDING * 2 + (rect.width - 170), LINE + 200,
        150, 60,
        [this](UI &ui)
        { save_druple(ui); }, "hoeveel?"));

    add_ui(std::make_unique<Button>(
        PEDING * 2 + (rect.width - 170), LINE + 120,
        150, 60,
        "save?",
        [this](UI &ui)
        { add_comment(ui); }));
    // add_ui(std::make_unique<Button>(1400, 10, 100, 50, "gek", close_glas));
    name = (Dropdown *)get_ui_at(0);
    amount = (TextInp *)get_ui_at(1);
}

void Glass::reset(void)
{
    ph = 0;
    mol = 0;
    bar.reset();
    this->pop_ui_back();
    this->pop_ui_back();
    this->sim.reset();
}

void Glass::reset_sim(void)
{
    this->sim.reset();

    for (auto &ion : bar.get_data())
    {
        sim.addParticles((int)(ion.val * 20), ion.col);
    }
}

std::string Glass::get_comment(void) const
{
    return ((TextInp *)get_ui_at(3))->get_text();
} 

void Glass::save_ion(Ion &ion, int amount)
{

    std::string &ion_name = ion.ion;
    Color col = data.get_color(ion.ion);
    float mol = ion.mol * amount;
    bar.add_value(ion_name, col, mol);
    sim.addParticles(int(mol * 20), col);
}

void Glass::save_druple(UI &ui)
{
    (void)ui;
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

    save_ion(elm->anion, a);
    save_ion(elm->kation, a);

    ph += elm->ph * a;
    mol += elm->mol * a;

    name->reset();
    amount->reset();
}

void Glass::add_comment(UI &ui)
{
    (void)ui;

    add_ui(std::make_unique<TextInp>(
        PEDING * 2, LINE + 300,
        rect.width - 200, 300, nullptr, "coment?"));

    add_ui(std::make_unique<Button>(
        PEDING * 2 + (rect.width - 170), LINE + 300,
        150, 300,
        "save?", next_glas_func));
}

// void Glass::draww(void) const
// {
//     BeginTextureMode(this->win);
//     clear();

//     DrawRectangleRounded(rect, 0.2, 8, LIGHTGRAY);
//     DrawRectangleRoundedLinesEx(rect, 0.2, 8, 6.0f, BLACK);
//     draw_my_text("pH: %.2f", ph, 1050, 10);
//     draw_my_text("mol: %.2f", mol, 1180, 10);
//     bar.draw(this->get_mouse_pos());
//     Win::draw();
//     EndTextureMode();
// }

void Glass::draw(void) const
{
    if (!_is_visible)
        return;
    DrawRectangleRounded(rect, ROUNDED, 10, COL_1);
    DrawRectangleRoundedLinesEx(rect, ROUNDED, 10, 6.0f, BLACK);
    draw_my_text("pH: %.2f", ph, PEDING * 3, LINE + 130);
    draw_my_text("mol: %.2f", mol, PEDING * 3 + 200, LINE + 130);
    bar.draw(this->get_mouse_pos());
    Win::draw();
}

bool Glass::take_code(std::string &code) const
{
    auto it = std::find(this->data.codes.begin(), this->data.codes.end(), code);
    if (it == this->data.codes.end())
        return false;

    int index = std::distance(this->data.codes.begin(), it);
    name->set(index);
    // todo drupel saven?
    return true;
}
