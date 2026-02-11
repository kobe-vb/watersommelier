#include "GlassModel.hpp"
#include <algorithm>
#include <format>

GlassModel::GlassModel(GameData &data, std::function<void()> score_glass, Sim &sim) : data(data), sim(sim)
{
    save_button = ButtonModel("Save", score_glass);
    _score_glass = score_glass;
    reset();
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
    volume = 0.025;
    id++;

    this->reset_sim();
    warning.clear();
}

void GlassModel::reset_sim(void)
{
    this->sim.reset();

    for (auto &ion : bar.get_data())
        sim.addParticles((int)(ion.val * SIM_FACTOR), ion.col);
}

void GlassModel::set_warning(void)
{
    std::string name = dropdown.get_selected_text();
    auto zout = data.get_element(name);

    const IonData &anion_data = data.get_ion_data(zout.anion.name);
    const IonData &kation_data = data.get_ion_data(zout.kation.name);

    double anion_mol = bar.contains(zout.anion.name) ? bar[zout.anion.name]->val : 0 ; // mol
    double kation_mol =  bar.contains(zout.kation.name) ? bar[zout.kation.name]->val : 0 ; // mol

    double delta_anion = anion_data.max_mol_per_glass - anion_mol;
    double delta_kation = kation_data.max_mol_per_glass - kation_mol; // dt mol

    double mol_per_druppel_anion = zout.m * zout.anion.n * 1 * VOLUME_DRUPEL;
    double mol_per_druppel_kation = zout.m * zout.kation.n * 1 * VOLUME_DRUPEL;

    double max_drupel_anion = delta_anion / mol_per_druppel_anion;
    double max_drupel_kation = delta_kation / mol_per_druppel_kation;

    double min = std::min(max_drupel_anion, max_drupel_kation);

    warning = std::format("Max: {:.0f} dr", min);
    std::cout << warning << std::endl;
}

void GlassModel::save_ion(Ion &ion, int amount, double M)
{
    double mol = M * ion.n * amount * VOLUME_DRUPEL;
    Color col = data.get_ion_data(ion.name).color;

    bar.add_value(ion.name, col, mol);
    sim.addParticles(std::max(1, int(mol * SIM_FACTOR)), col);
}

void GlassModel::save_drops(int drops, Element *salt)
{

    previous_drops = drops;
    previous_salt = salt;

    volume += drops * VOLUME_DRUPEL;
    osmo += (drops * VOLUME_DRUPEL * salt->m * (salt->anion.n + salt->kation.n));
    
    save_ion(salt->anion, drops, salt->m);
    save_ion(salt->kation, drops, salt->m);
}

void GlassModel::remove_previous_drops(void)
{
    if (previous_salt == nullptr)
        return;

    volume -= previous_drops * VOLUME_DRUPEL;
    osmo -= (previous_drops * VOLUME_DRUPEL * previous_salt->m * (previous_salt->anion.n + previous_salt->kation.n));

    save_ion(previous_salt->anion, -previous_drops, previous_salt->m);
    save_ion(previous_salt->kation, -previous_drops, previous_salt->m);

    previous_drops = 0;
    previous_salt = nullptr;
    reset_sim();
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
        Element &zout = data.get_element(data.names[std::rand() % data.names.size()]);

        save_drops(a, &zout);
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
