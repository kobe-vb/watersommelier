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

    StackedBar(const StackedBar &other);
    StackedBar &operator=(const StackedBar &other);

    void set_pos(int x, int y);
    void reset(void);

    void add_value(const std::string &name, Color col, float val);
    void draw(Vector2 mouse) const;

    const std::vector<Data> &get_data(void) const { return data; }
    float get_total_volume(void) const { return total_volume; }
};
