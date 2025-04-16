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

class StackedBar
{
private:
    std::vector<Data> data;
    float total_volume = 0;
    float x, y, width, height;

public:
    StackedBar() = default;
    StackedBar(int x, int y, int w, int h);
    ~StackedBar() = default;

    void add_value(const std::string &name, Color col, float val);
    void draw() const;
};
