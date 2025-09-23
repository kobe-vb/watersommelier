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
#include <regex>

static void draw_my_text(const char *name, float val, int x, int y)
{
    char buffer[64];
    snprintf(buffer, sizeof(buffer), name, val);
    DrawText(buffer, x, y, 30, DARKBLUE);
}

Glass::Glass(GameData &data, std::function<void(UI &)> close_glas, Sim &sim, Rectangle &rect) : rect(rect), data(data), sim(sim), next_glas_func(close_glas)
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
        "ik drink",
        [this](UI &ui)
        { add_comment(ui); }));
    // add_ui(std::make_unique<Button>(1400, 10, 100, 50, "gek", close_glas));
    name = (Dropdown *)get_ui_at(0);
    amount = (TextInp *)get_ui_at(1);
}

void Glass::reset(void)
{
    osmo = 0;
    volume = 25;
    bar.reset();
    this->pop_ui_back();
    this->pop_ui_back();
    this->sim.reset();

    for (size_t i = 0; i < hastags.size() - 1; ++i)
        pop_ui_back();
    hastags.clear();
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

void Glass::save_ion(Ion &ion, int amount, float M)
{
    float molarity = M * amount * 0.000055 * ion.Nat;
    Color col = data.get_ion_data(ion.name).color;

    bar.add_value(ion.name, col, molarity);
    sim.addParticles(int(amount * 3 * ion.Nat), col);
















    // std::string &ion_name = ion.ion;
    // Color col = data.get_color(ion.ion);
    // float mol = ion.mol * amount;
    // bar.add_value(ion_name, col, mol);
    // sim.addParticles(int(mol * 20), col);

    // float masa = molMasa * ion.molFactor * concentration * volumePipet * droplets;
}

void Glass::save_druple(UI &ui)
{
    (void)ui;

    int number_of_druplets = 0;
    Element *elm;
    try
    {
        number_of_druplets = std::stoi(amount->get_text());
        elm = &data.get_element(name->get_selected_text());
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << '\n';
        return;
    }

    volume += number_of_druplets * 0.055;
    osmo += number_of_druplets * elm->M;


    save_ion(elm->anion, number_of_druplets, elm->M);
    save_ion(elm->kation, number_of_druplets, elm->M); 









    // save_ion(elm->anion, droplet);  // --> (masa / volume) * i *  = osmaFraction
    // save_ion(elm->kation, droplet); // --> (masa / volume) * i = osmaFraction

    // operOsmo += anion(osmaFraction) + katon(osmaFraction)

    // mol += (elm->mol * droplet * concentration); // osmo

    name->reset();
    amount->reset();
}

void Glass::add_comment(UI &ui)
{
    (void)ui;
    _add_comment();
}

void Glass::_add_comment(void)
{
    add_ui(std::make_unique<TextInp>(
        PEDING * 2, LINE + 300,
        rect.width - 200, 300, nullptr, "coment?"));
    // test
    dynamic_cast<TextInp &>(*get_ui_at(get_num_of_elements() - 1)).set_text("vdf #one #two #three");

    add_ui(std::make_unique<Button>(
        PEDING * 2 + (rect.width - 170), LINE + 300,
        150, 300,
        "score #", [this](UI &ui)
        { (void)ui; _add_score(); }));
}

void Glass::_add_score(void)
{
    comment = get_comment();
    std::regex hashtagRegex("#\\w+");

    auto it = std::sregex_iterator(comment.begin(), comment.end(), hashtagRegex);
    auto end = std::sregex_iterator();

    for (; it != end; ++it)
        hastags[it->str()] = 0;

    this->pop_ui_back();
    this->pop_ui_back();

    add_ui(std::make_unique<Button>(
        PEDING * 2 + (rect.width - 170), LINE + 300,
        150, 300,
        "save glass", [this](UI &ui)
        { (void)ui; _next_glass(ui); }));

    int index = 0;
    for (auto &tag : hastags)
    {
        add_ui(std::make_unique<TextInp>(
            PEDING * 2,
            LINE + 300 + index * 60,
            rect.width - 200,
            30,
            nullptr,
            tag.first));
        ((TextInp *)(get_ui_at(get_num_of_elements() - 1)))->set_text("5"); /////////////
        index++;
    }
}

void Glass::_next_glass(UI &ui)
{
    bool is_valid = true;
    int i = get_num_of_elements() - hastags.size();
    for (auto &tag : hastags)
    {
        bool valid = true;
        TextInp *text = (TextInp *)get_ui_at(i++);
        try
        {
            tag.second = std::stoi(text->get_text());
            if (tag.second <= 0 || tag.second > 5)
                valid = false;
        }
        catch (...)
        {
            valid = false;
        }
        text->set_bg_color(valid ? GREEN : RED);
        is_valid &= valid;
    }
    if (!is_valid)
        return;
    next_glas_func(ui);
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
    draw_my_text("mol: %.2f", osmo, PEDING * 3 + 200, LINE + 130);
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

void Glass::generate_random_data(bool full)
{
    for (int i = 0; i < 10; i++)
    {
        int a = std::rand() % 5 + 1;
        Element &elm = data.get_element(data.names[std::rand() % data.names.size()]);

        save_ion(elm.anion, a, elm.M);
        save_ion(elm.kation, a, elm.M);

        // ph += elm.ph * a;
        // mol += elm.mol * a;
    }
    if (!full)
        return;
    _add_comment();
    const char *texts[] = {"woow #sick #leker", "cool #ba", "very cool #nice_waterrr"};
    ((TextInp *)get_ui_at(3))->set_text(texts[std::rand() % 3]);
}