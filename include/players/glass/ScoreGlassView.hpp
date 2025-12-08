#pragma once

#include "Win.hpp"
#include "ScoreGlassModel.hpp"
#include "ButtonView.hpp"
#include "TextInpView.hpp"


class ScoreGlassView: public Win
{
    private:
        ScoreGlassModel *model;
        const Rectangle &rect;

        void add_score(void);
        bool valid_score(void);
        void save(void);

    public:
        ScoreGlassView(ScoreGlassModel *model, Rectangle &rect);
        ~ScoreGlassView() = default;

        void draw(void) const override;

        void reset(void);
};