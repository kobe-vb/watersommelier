#pragma once
#include <string>
#include <vector>
#include "raylib.h"
#include "StackedBarModel.hpp"


class StackedBarHView
{
private:
    StackedBarModel *model;
    float x, y, width, height;

public:
    StackedBarHView () = default;
    StackedBarHView (StackedBarModel *model, int x, int y, int w, int h);
    ~StackedBarHView() = default;

    StackedBarHView(const StackedBarHView &other);
    StackedBarHView &operator=(const StackedBarHView &other);

    void set_pos(int x, int y);
    void draw(Vector2 mouse) const;
};
