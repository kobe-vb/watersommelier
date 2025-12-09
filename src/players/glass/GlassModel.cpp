#include "GlassModel.hpp"
#include <algorithm>

GlassModel::GlassModel(GameData &data, std::function<void()> score_glass, Sim &sim) : data(data), sim(sim)
{
    save_button = ButtonModel("Save", score_glass);
    _score_glass = score_glass;

}

void GlassModel::lock(bool lock)
{
    save_button.set_locked(lock);
    dropdown.set_locked(lock);
    amount.set_locked(lock);
}

void GlassModel::reset(void)
{
    lock(false);
    bar.reset();
    osmo = 0;
    volume = 25;
    id++;

    // this->sim.reset();
    // TODO
    // hastags.clear();
    // comment.clear();
    warning.clear();
}

void GlassModel::reset_sim(void)
{
    // this->sim.reset();

    // for (auto &ion : bar.get_data())
    //     sim.addParticles((int)(ion.val * SIM_FACTOR), ion.col);
    // std::cout << "reset sim with " << bar.get_data().size() << " ions\n";
}

void GlassModel::set_warning(void)
{
    std::string name = dropdown.get_selected_text();

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

    float delta_ion = ion.max_glass_mg - ((ion_val * ion.gram_per_mol));
    float delta_kion = kion.max_glass_mg - ((kion_val * kion.gram_per_mol)); // dt mg

    float massa_one_druple_i = (elm.Mol_per_liter_per_zout * elm.anion.n * ion.gram_per_mol * 0.000055); // mg
    float massa_one_druple_k = (elm.Mol_per_liter_per_zout * elm.kation.n * kion.gram_per_mol * 0.000055);

    float max_drupel_ion = (delta_ion) / (massa_one_druple_i);
    float max_drupel_kion = (delta_kion) / (massa_one_druple_k);

    float min = std::min(max_drupel_ion, max_drupel_kion);

    warning = "nog max " + std::to_string((int)min) + " drupels";
}

void GlassModel::save_ion(Ion &ion, int amount, float M)
{
    float molarity = M * amount * 0.000055 * ion.n;
    Color col = data.get_ion_data(ion.name).color;

    bar.add_value(ion.name, col, molarity);
    // sim.addParticles(int(molarity * SIM_FACTOR), col);
}

void GlassModel::save_drops(int drops, Element *elm)
{
    volume += drops * 0.055;
    osmo += drops * 40 * elm->dosdr;
    // osmo += number_of_druplets * 40 * (elm->anion.Nat + elm->kation.Nat) * elm->M;

    save_ion(elm->anion, drops, elm->Mol_per_liter_per_zout);
    save_ion(elm->kation, drops, elm->Mol_per_liter_per_zout);
}

void GlassModel::save_drops(void)
{
    save_drops(get_new_amount(), &get_element());

    dropdown.reset();
    amount.reset();
    amount.set_active(false);
    warning.clear();
}

int GlassModel::get_new_amount(void)
{
    const std::string &text = amount.get_text();
    if (text.empty())
        return 1;
    return std::stoi(text);
}

Element &GlassModel::get_element(void)
{
    return data.get_element(dropdown.get_selected_text());
}

bool GlassModel::take_code(const std::string &code)
{
    auto it = std::find(this->data.codes.begin(), this->data.codes.end(), code);
    if (it == this->data.codes.end())
        return false;

    int index = std::distance(this->data.codes.begin(), it);
    dropdown.set(index);
    set_warning();
    amount.set_active(true);

    // dropdown->clear_color(UiColors::BG);

    // todo drupel saven?
    return true;
}

void GlassModel::generate_random_data(bool full)
{
    for (int i = 0; i < 8; i++)
    {
        int a = std::rand() % 15 + 10;
        Element &elm = data.get_element(data.names[std::rand() % data.names.size()]);

        save_drops(a, &elm);
    }
    if (!full)
        return;
    // _add_comment();

    // std::string comment;
    // std::vector<std::string> opties = {"#flats", "#bitter", "#salty"};

    // int max = std::rand() % 2 + 1;
    // for (int i = 0; i < max; i++)
    //     comment += opties[std::rand() % opties.size()] + " ";

    // ((TextInp *)get_ui_at(3))->set_text(comment);
    // _add_score();
    // for (auto &tag : hastags)
    //     tag.second = std::rand() % 5 + 1;
}
