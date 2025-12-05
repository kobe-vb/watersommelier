#pragma once

#include <map>
#include <string>

#include "Win.hpp"
#include "ButtonView.hpp"
#include "TextInpView.hpp"
#include "DropdownView.hpp"
#include "GameData.hpp"
#include "StackedBarHView.hpp"
#include "Sim.hpp"
#include "GlassModel.hpp"

class GlassView : public Win
{
private:

    GlassModel *model;

    Rectangle &rect;
    StackedBarHView bar;

    TextInpView *amount;
    DropdownView *name;

    void process_drops();
    void score_glass();

public:
    GlassView(GlassModel *model, Rectangle &rect);
    ~GlassView() = default;

    void reset(void);

    void draw(void) const override;

    void lock(bool lock=true);

};