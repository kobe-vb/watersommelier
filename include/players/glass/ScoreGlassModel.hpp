#pragma once

#include <map>

#include "TextInpModel.hpp"
#include "ButtonModel.hpp"

class ScoreGlassModel: public UIModel
{

private:
    TextInpModel comment_input = TextInpModel("type Comment");
    ButtonModel score_button = ButtonModel("score #", [this]()
                                           { add_score(); });

    ButtonModel save_button = ButtonModel("Save glass", [this]()
                                          { next_glass(); });
    TextInpModel thief_input = TextInpModel("type code/name Thief", [this]()
                                            { steal_glass(); });

    void add_score(void);

    std::function<void()> next_glass;
    std::function<bool()> steal_glass;

public:
    ScoreGlassModel(std::function<void()> next_glass, std::function<bool()> steal_glass);
    ~ScoreGlassModel() = default;

    TextInpModel &get_comment(void) { return comment_input; }
    ButtonModel &get_score_button(void) { return score_button; }
    ButtonModel &get_save_button(void) { return save_button; }
    TextInpModel &get_thief_input(void) { return thief_input; }
    std::map<std::string, TextInpModel> hastags;

    int get_hastag_value(const TextInpModel &inp) const;

    std::string const &get_comment_text(void) const { return comment_input.get_text(); }
    std::map<std::string, int> get_hastags(void) const;
};