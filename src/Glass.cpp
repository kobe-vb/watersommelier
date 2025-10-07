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
        data.names, "lol", [this](UI &ui)
        { _set_warning(ui); }));

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
    this->sim.reset();

    if (!hastags.empty())
        for (size_t i = 0; i < hastags.size(); ++i)
            pop_ui_back();
    hastags.clear();
    _set_lock(false);
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

void Glass::_set_warning(UI &ui)
{
    std::string name = ((Dropdown &)(ui)).get_selected_text();
    auto elm = data.get_element(name);
    auto elements = bar.get_data();

    auto &ion = data.get_ion_data(elm.anion.name);
    auto &kion = data.get_ion_data(elm.kation.name);

    float ion_val = 0; //  som van (M * amount * 0.000055 * ion.n) // mol
    float kion_val = 0;
    for (auto &element : elements)
    {
        if (element.name == elm.anion.name)
            ion_val = element.val;
        if (element.name == elm.kation.name)
            kion_val = element.val;
    }

    float delta_ion = ion.max_glass_mg - ((ion_val * ion.gram_per_mol) * 1000);
    float delta_kion = kion.max_glass_mg - ((kion_val * kion.gram_per_mol) * 1000); // dt mg

    float massa_one_druple_i = (elm.Mol_per_liter_per_zout * elm.anion.n * ion.gram_per_mol); // mg
    float massa_one_druple_k = (elm.Mol_per_liter_per_zout * elm.kation.n * kion.gram_per_mol);

    float max_drupel_ion = (delta_ion * 1) / (massa_one_druple_i * 10);
    float max_drupel_kion = (delta_kion * 1) / (massa_one_druple_k * 10);

    float min = std::min(max_drupel_ion, max_drupel_kion);

    warning = "nog max " + std::to_string((int)min) + " drupels";
}

void Glass::save_ion(Ion &ion, int amount, float M)
{
    float molarity = M * amount * 0.000055 * ion.n;
    Color col = data.get_ion_data(ion.name).color;

    bar.add_value(ion.name, col, molarity);
    sim.addParticles(int(amount * 3 * ion.n), col);
}

void Glass::_save_druple(int number_of_druplets, Element *elm)
{
    volume += number_of_druplets * 0.055;
    osmo += number_of_druplets * 40 * elm->dosdr;
    // osmo += number_of_druplets * 40 * (elm->anion.Nat + elm->kation.Nat) * elm->M;

    save_ion(elm->anion, number_of_druplets, elm->Mol_per_liter_per_zout);
    save_ion(elm->kation, number_of_druplets, elm->Mol_per_liter_per_zout);

}

void Glass::save_druple(UI &ui)
{
    (void)ui;

    int number_of_druplets = 0;
    Element *elm;
    try
    {
        number_of_druplets = std::stoi(amount->get_text());
    }
    catch (const std::exception &e)
    {
        amount->set_color(UiColors::BG, RED);
        std::cerr << e.what() << '\n';
        return;
    }
    amount->clear_color(UiColors::BG);

    try
    {
        elm = &data.get_element(name->get_selected_text());
    }
    catch (const std::exception &e)
    {
        name->set_color(UiColors::BG, RED);
        std::cerr << e.what() << '\n';
        return;
    }
    name->clear_color(UiColors::BG);

    _save_druple(number_of_druplets, elm);

    name->reset();
    amount->reset();
}

void Glass::add_comment(UI &ui)
{
    (void)ui;
    _add_comment();
}

void Glass::_set_lock(bool lock)
{
    for (int i = 0; i < get_num_of_elements(); ++i)
        get_ui_at(i)->set_lock(lock);
}

void Glass::_add_comment(void)
{
    _set_lock(true);
    add_ui(std::make_unique<TextInp>(
        PEDING * 2, LINE + 300,
        rect.width - 200, 300, nullptr, "coment?"));
    
    if (DEBUG)
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
        if (DEBUG)
            ((TextInp *)(get_ui_at(get_num_of_elements() - 1)))->set_text("5");
        index++;
    }

    add_ui(std::make_unique<Button>(
        PEDING * 2 + (rect.width - 170), LINE + 300,
        150, 300,
        "save glass", [this](UI &ui)
        { (void)ui; _next_glass(ui); }));

}

void Glass::_next_glass(UI &ui)
{
    bool is_valid = true;
    int i = get_num_of_elements() - hastags.size() - 1;
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
        text->set_color(UiColors::BG, valid ? GREEN : RED);
        text->set_on_focus_clear(true);
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
    draw_my_text("mol: %.2f", osmo, PEDING * 3 + 50, LINE + 130);
    DrawText(warning.c_str(), PEDING * 3 + 200, LINE + 130, 30, RED);
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
    for (int i = 0; i < 8; i++)
    {
        int a = std::rand() % 15 + 10;
        Element &elm = data.get_element(data.names[std::rand() % data.names.size()]);

        _save_druple(a, &elm);
    }
    if (!full)
        return;
    _add_comment();

    std::string comment;
    std::vector<std::string> opties = {"#flats", "#bitter", "#salty"};

    int max = std::rand() % 2 + 1;
    for (int i = 0; i < max; i++)
        comment += opties[std::rand() % opties.size()] + " ";

    ((TextInp *)get_ui_at(3))->set_text(comment);
    _add_score();
    for (auto &tag : hastags)
        tag.second = std::rand() % 5 + 1;
}