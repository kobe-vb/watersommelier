#pragma once
#include <string>
#include <vector>
#include "raylib.h"

struct Data
{
    std::string name;
    Color col;
    float val;
};

class StackedBarModel
{
private:
    std::vector<Data> data;
    float total_volume = 0;

public:
    StackedBarModel() = default;
    ~StackedBarModel() = default;
    StackedBarModel(const StackedBarModel &other) = default;
    StackedBarModel &operator=(const StackedBarModel &other) = default;

    void reset(void);
    void add_value(const std::string &name, Color col, float val);
    const std::vector<Data> &get_data(void) const { return data; }
    float get_total_volume(void) const { return total_volume; }

    Data *operator[](const std::string& name);
    bool contains(const std::string& name) const;
};