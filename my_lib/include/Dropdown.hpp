#pragma once
#include "UI.hpp"
#include <string>
#include <vector>
#include <functional>

#include <algorithm>

class Dropdown : public UI {
private:
    Rectangle bounds;
    std::string input_text;
    std::vector<std::string> options;
    std::vector<std::string> filtered;
    bool is_open;
    std::string placeholder;
    int max_opt = 5;
    int ind = 0;
    int current_ind = 0;

public:
    Dropdown(float x, float y, float w, float h, const std::vector<std::string>& opts, const std::string& ph = "", std::function<void(UI&)> cb = nullptr);

    void update() override;
    void draw() const override;
    std::string get_selected_text() const;
    int get_selected_ind() const;
    void set_max_opt(int i);
    void reset(void);

private:
    void filter_options();
    void handle_text();
    bool capture_tab(void) override;
};
