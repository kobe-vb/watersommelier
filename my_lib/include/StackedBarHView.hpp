#pragma once
#include <string>
#include <vector>
#include <functional>

#include "raylib.h"
#include "StackedBarModel.hpp"

class StackedBarHView
{
private:
    StackedBarModel *model;
    float x, y, width, height;

    std::function<double(double)> value_converter = [](double x) { return x; };
    std::string unit = "";
    bool show_info = false;

public:
    StackedBarHView () = default;
    StackedBarHView (StackedBarModel *model, int x, int y, int w, int h);
    ~StackedBarHView() = default;

    StackedBarHView(const StackedBarHView &other);
    StackedBarHView &operator=(const StackedBarHView &other);

    void set_pos(int x, int y);

    void draw(Vector2 mouse) const;
    void update(Vector2 mouse); 

    void set_converter(std::function<double(double)> fn, const std::string& u);
};
