#include "StackedBarModel.hpp"

void StackedBarModel::add_value(const std::string &name, Color col, float val)
{
    for (auto &segment : data)
    {
        if (segment.name == name)
        {
            segment.val += val;
            total_volume += val;
            return;
        }
    }
    data.push_back({name, col, val});
    total_volume += val;
}

void StackedBarModel::reset(void)
{
    data.clear();
    total_volume = 0;
}