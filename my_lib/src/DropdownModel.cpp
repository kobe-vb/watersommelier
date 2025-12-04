#include "DropdownModel.hpp"
#include <algorithm>

DropdownModel::DropdownModel(const std::vector<std::string> &opts, const std::string &ph, std::function<void()> cb) : UIModel(cb), placeholder(ph), options(opts), filtered(opts) 
{
}

void DropdownModel::filter_options()
{
    filtered.clear();
    for (const auto &opt : options)
    {
        if (opt.find(input_text) != std::string::npos)
            filtered.push_back(opt);
    }
}

std::string DropdownModel::get_selected_text() const
{
    return input_text;
}

int DropdownModel::get_selected_ind() const
{
    // todo
    return (0);
}

void DropdownModel::set_max_opt(int i)
{
    max_opt = i;
}

void DropdownModel::reset(void)
{
    input_text.clear();
    ind = 0;
    current_ind = 0;
}

void DropdownModel::set(int index)
{
    input_text = options[index];
    ind = 0;
    current_ind = 0;
}

void DropdownModel::add_char(char c)
{
    input_text += c;
}

void DropdownModel::remove_char(void)
{
    input_text.pop_back();
}

bool DropdownModel::is_empty() const
{
    return input_text.empty();
}

void DropdownModel::on_change(void)
{
    filter_options();
    ind = 0;
    current_ind = 0;
}

void DropdownModel::open()
{
    filter_options();
    ind = 0;
    current_ind = 0;
    _open = true;
}

void DropdownModel::close()
{
    _open = false;
}

void DropdownModel::move_up()
{
    if (current_ind == 0)
        ind--;
    else
        current_ind--;
}

void DropdownModel::move(int direction)
{
    ind = std::clamp(ind + direction, 0, std::max(0, (int)filtered.size() - 1 - max_opt));
}

void DropdownModel::move_down()
{
    if (current_ind == (max_opt - 1))
        ind++;
    else
        current_ind++;
}

void DropdownModel::select_current()
{
    if (filtered.size() == 1)
    {
        _open = false;
        run_callback();
    }
    input_text = filtered[current_ind + ind];
    filter_options();
    ind = 0;
    current_ind = 0;
}

void DropdownModel::select(int index)
{
    input_text = filtered[index + ind];
    filter_options();
    _open = false;
    run_callback();
}