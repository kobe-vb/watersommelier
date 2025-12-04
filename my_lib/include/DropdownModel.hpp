#pragma once

#include <string>
#include <vector>
#include <functional>

#include "UIModel.hpp"

class DropdownModel : public UIModel
{
private:
    std::string placeholder;
    std::vector<std::string> options;
    std::vector<std::string> filtered;
    std::string input_text;
    int ind = 0;
    int current_ind = 0;
    bool _hover = false;
    bool _open = false;
    int max_opt = 5;

    void filter_options();

public:
    DropdownModel(const std::vector<std::string> &opts, const std::string &ph = "", std::function<void()> cb = nullptr);
    virtual ~DropdownModel() {}

    std::string get_selected_text() const;
    int get_selected_ind() const;
    int get_visible_size(void) const { return (int)filtered.size(); }
    int get_max_opt() const { return max_opt; }
    int get_current_index() const { return current_ind; }
    int get_index() const { return ind; }
    const std::string &get_placeholder() const { return placeholder; }
    const std::string &get_input_text() const { return input_text; }
    const std::string &get_option(int i) const { return filtered[i + ind]; }

    void set_max_opt(int i);
    void set(int index);
    void set_hover(bool value) { _hover = value; }

    bool is_hover() const { return _hover; }
    bool is_open() const { return _open; }

    void add_char(char c);
    void remove_char(void);

    bool is_empty() const;

    void reset(void);
    void on_change(void);
    void open();
    void close();
    void move_down(void);
    void move_up(void);
    void select_current(void);
    void select(int i);
    void move(int i);

};