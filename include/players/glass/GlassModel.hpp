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

#define SIM_FACTOR 100000000

class GlassModel
{
    friend class HistoryGlass;

private:
    GameData &data;
    Sim &sim;

    StackedBarModel bar;
    float osmo = 0;
    float volume = 25;

    std::string warning;

    ButtonModel save_button;
    DropdownModel dropdown = DropdownModel(data.names, "Select a posion", [this]()
                             { set_warning(); });
    TextInpModel amount = TextInpModel("??hoeveel??", [this]()
                          { save_drops(); });

    void save_ion(Ion &ion, int amount, float M);
    // te doen
    
    void _next_glass(UI &ui);
    void save_drops(void);
    
    public:
    GlassModel(GameData &data, std::function<void()> close_glass, Sim &sim);
    ~GlassModel() {};
    
    StackedBarModel &get_bar(void) { return bar; }
    ButtonModel &get_save_button(void) { return save_button; }
    DropdownModel &get_dropdown(void) { return dropdown; }
    TextInpModel &get_amount(void) { return amount; }
    
    int get_new_amount(void);
    Element &get_element(void);
    
    void reset(void);
    void reset_sim(void);
    
    void set_warning(void);
    void save_drops(int drops, Element *elm);
    
    int get_osmo(void) const { return osmo; }
    const std::string &get_warning(void) const { return warning; }
    bool take_code(const std::string &code);
    // te doen


    void add_comment(UI &ui);

    std::string get_comment(void) const;

    // void generate_random_data(bool full = true);
};