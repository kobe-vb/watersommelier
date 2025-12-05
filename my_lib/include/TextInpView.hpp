#pragma once

#include "UIView.hpp"
#include "TextInpModel.hpp"

#include <sstream>
#include <string>

class TextInpView : public UIView
{
private:
    TextInpModel *model;
    Rectangle bounds;

    void set_active(bool value);

public:
    TextInpView(TextInpModel *model, float x, float y, float w, float h);
    ~TextInpView() = default;

    void draw(void) const override;
    bool update(void) override;

    bool capture_tab(int direction) override;

    const Rectangle &get_rect(void);
    void move(int x, int y);
};