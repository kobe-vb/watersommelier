#pragma once

#include <string>

#include "Win.hpp"
#include "ButtonModel.hpp"
#include "TextInpModel.hpp"
#include "DropdownModel.hpp"
#include "GameData.hpp"
#include "StackedBarModel.hpp"
#include "Sim.hpp"
#include "GlassModel.hpp"

#define SIM_FACTOR 100000

class GlassModel : public UIModel
{
    friend class HistoryGlass;

private:
    GameData &data;
    Sim &sim;

    StackedBarModel bar;
    double osmo = 0;
    double volume = 25;
    int id = 0;

    std::string warning;

    ButtonModel save_button;
    std::function<void()> _score_glass;
    DropdownModel dropdown = DropdownModel(data.names, "Select a flavor", [this]()
                                           { set_warning(); });
    TextInpModel amount = TextInpModel("how many", [this]()
                                       { save_drops(); });

    void save_ion(Ion &ion, int amount, double M);
    void save_drops(int drops, Element *elm);

    int previous_drops = 0;
    Element *previous_salt = nullptr;

public:
    GlassModel(GameData &data, std::function<void()> score_glass, Sim &sim);
    ~GlassModel() {};

    StackedBarModel &get_bar(void) { return bar; }
    ButtonModel &get_save_button(void) { return save_button; }
    DropdownModel &get_dropdown(void) { return dropdown; }
    TextInpModel &get_amount(void) { return amount; }

    int get_new_amount(void);
    Element &get_element(void);

    void reset(void);
    void reset_sim(void);

    void inc(void) { id++; };

    void set_warning(void);
    void save_drops(void);
    void score_glass(void) { _score_glass(); }

    double get_osmo(void) const { return osmo; }
    double get_volume(void) const { return volume; } 
    int get_id(void) const { return id; }
    const std::string &get_warning(void) const { return warning; }
    bool take_code(const std::string &code);

    void lock(bool lock = true);

    void generate_random_data(bool full = true);

    void remove_previous_drops(void);
};