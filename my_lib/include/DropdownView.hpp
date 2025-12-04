#pragma once
#include <string>
#include <vector>
#include <functional>
#include <algorithm>

#include "UI.hpp"
#include "DropdownModel.hpp"

class DropdownView : public UI
{
private:
    DropdownModel *model;
    Rectangle bounds;

    bool capture_tab(int direction) override;
    void handle_text();

public:
    DropdownView(DropdownModel *model, float x, float y, float w, float h);
    ~DropdownView() = default;

    bool update() override;
    void draw() const override;
};
