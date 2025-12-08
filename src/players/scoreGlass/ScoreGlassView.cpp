#include "ScoreGlassView.hpp"
#include "Settings.hpp"

ScoreGlassView::ScoreGlassView(ScoreGlassModel *model, Rectangle &rect) : Win(model), model(model), rect(rect)
{
    // void Glass::_add_comment(void)
    // _set_lock(true);
    reset();
}

void ScoreGlassView::reset(void)
{
    clear_ui();

    int x = rect.x + rect.width - PEDING - BUTTON_WIDTH;
    int y = rect.y + (BUTTON_HEIGHT * 3) + (PEDING * 4);
    int h = rect.y + rect.height - y - PEDING * 2 - BUTTON_HEIGHT;

    add_ui(std::make_unique<TextInpView>(
        &model->get_comment(),
        rect.x + PEDING, y,
        rect.width - PEDING * 2, h));

    y += h + PEDING;
    add_ui(std::make_unique<ButtonView>(
        &model->get_score_button(),
        x, y,
        BUTTON_WIDTH, BUTTON_HEIGHT));
    model->get_score_button().set_callback([this]() { this->add_score(); });
}

void ScoreGlassView::add_score(void)
{

    model->add_score();

    this->set_current_tab(this->get_num_of_elements() - 1);
    this->clear_ui();

    int x = rect.x + rect.width - PEDING - BUTTON_WIDTH;
    int y = rect.y + (BUTTON_HEIGHT * 3) + (PEDING * 4);
    int w = (rect.width - PEDING * 3) / 2;

    int index = 0;
    for (auto &tag : model->hastags)
    {
        add_ui(std::make_unique<TextInpView>(
            &tag.second,
            rect.x + PEDING + (w + PEDING) * (int)(index / 4),
            y + (index % 4) * 50,
            w, 40));
        index++;
    }

    y += rect.y + rect.height - y - PEDING - BUTTON_HEIGHT;
    add_ui(std::make_unique<ButtonView>(
        &model->get_save_button(),
        x, y,
        BUTTON_WIDTH, BUTTON_HEIGHT));
    model->get_save_button().set_callback([this]() { this->save(); });

    add_ui(std::make_unique<TextInpView>(
        &model->get_thief_input(),
        rect.x + PEDING, y,
        rect.width - BUTTON_WIDTH - PEDING * 3, BUTTON_HEIGHT));
}

bool ScoreGlassView::valid_score(void)
{
    bool is_valid = true;
    int i = 0;
    for (auto &tag : model->hastags)
    {
        bool valid = model->get_hastag_value(tag.second) != -1;
        TextInpView *text = dynamic_cast<TextInpView *>(get_ui_at(i++));
        text->set_color(UiColors::BG, valid ? GREEN : RED);
        tag.second.set_on_focus_clear(true);
        is_valid &= valid;
    }
    return is_valid;
}


void ScoreGlassView::save()
{
    if (!valid_score())
        return;
    model->next_glass();
    this->reset();
}

void ScoreGlassView::draw(void) const
{
    if (!model->is_visible())
        return;

    Win::draw();
}