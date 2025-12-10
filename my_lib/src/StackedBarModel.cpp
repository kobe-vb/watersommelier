#include "StackedBarModel.hpp"

Data *StackedBarModel::operator[](const std::string& name)
{
    for (auto &segment : data)
    {
        if (segment.name == name)
        {
            return &segment;
        }
    }
    return nullptr;
}

bool StackedBarModel::contains(const std::string& name) const
{
    for (const auto &segment : data)
    {
        if (segment.name == name)
            return true;
    }
    return false;
}

void StackedBarModel::add_value(const std::string &name, Color col, float val)
{
    Data *existing_data = (*this)[name];
    if (existing_data != nullptr)
    {
        existing_data->val += val;
        total_volume += val;
        return;
    }
    data.push_back({name, col, val});
    total_volume += val;
}

void StackedBarModel::reset(void)
{
    data.clear();
    total_volume = 0;
}