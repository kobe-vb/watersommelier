/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Glass.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kvanden- <kvanden-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/14 16:31:31 by kvanden-          #+#    #+#             */
/*   Updated: 2025/05/20 20:33:55 by kvanden-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "GlassView.hpp"
#include "Settings.hpp"
#include "MyDraw.hpp"

static void draw_my_text(const char *name, float val, int x, int y)
{
    char buffer[64];
    snprintf(buffer, sizeof(buffer), name, val);
    DrawText(buffer, x, y, 30, BLACK);
}

GlassView::GlassView(GlassModel *model, Rectangle &rect) : Win(model), model(model), rect(rect)
{

    rect.x = UI_BORDER * 2;
    rect.y = LINE;
    rect.width = ((GetScreenWidth() - (UI_BORDER * 4) - (PEDING * 2)) * 1 / 3);
    rect.height = GetScreenHeight() - LINE - PEDING - UI_BORDER;

    bar = StackedBarHView(&model->get_bar(), rect.x + PEDING, rect.y + PEDING, rect.width - PEDING * 2, BUTTON_HEIGHT);

    int x = rect.x + rect.width - PEDING - BUTTON_WIDTH;
    int y = rect.y + PEDING + BUTTON_HEIGHT + PEDING;
    add_ui(std::make_unique<ButtonView>(
        &model->get_save_button(),
        x, y,
        BUTTON_WIDTH, BUTTON_HEIGHT));
    y += BUTTON_HEIGHT + PEDING;

    add_ui(std::make_unique<DropdownView>(
        &model->get_dropdown(),
        rect.x + PEDING, y,
        rect.width - BUTTON_WIDTH - PEDING * 3, BUTTON_HEIGHT));
    name = static_cast<DropdownView *>(get_last_ui());

    add_ui(std::make_unique<TextInpView>(
        &model->get_amount(),
        x, y,
        BUTTON_WIDTH, BUTTON_HEIGHT));
    amount = static_cast<TextInpView *>(get_last_ui());

    model->get_amount().set_callback([this] { this->process_drops(); });
    model->get_save_button().set_callback([this] { this->score_glass(); });
}

void GlassView::reset(void)
{
    model->reset();
    // _set_lock(false);

    // te doen
    // this->pop_ui_back();
    // this->pop_ui_back();

    // if (!hastags.empty())
    //     for (size_t i = 0; i < hastags.size(); ++i)
    //         pop_ui_back();
    // hastags.clear();
}

void GlassView::process_drops(void)
{
    try
    {
        model->get_new_amount();
    }
    catch (const std::exception &e)
    {
        amount->set_color(UiColors::BG, RED);
        std::cerr << e.what() << '\n';
        return;
    }
    amount->clear_color(UiColors::BG);

    try
    {
        model->get_element();
    }
    catch (const std::exception &e)
    {
        name->set_color(UiColors::BG, RED);
        std::cerr << e.what() << '\n';
        return;
    }
    name->clear_color(UiColors::BG);

    model->save_drops();
}

void GlassView::score_glass(void)
{
    model->lock();
    model->score_glass();
}

// heel de win locken?
// void GlassView::set_lock(bool lock)
// {
//     for (int i = 0; i < get_num_of_elements(); ++i)
//         get_ui_at(i)->set_lock(lock);
// }


// void Glass::draww(void) const
// {
//     BeginTextureMode(this->win);
//     clear();

//     DrawRectangleRounded(rect, 0.2, 8, LIGHTGRAY);
//     DrawRectangleRoundedLinesEx(rect, 0.2, 8, 6.0f, BLACK);
//     draw_my_text("pH: %.2f", ph, 1050, 10);
//     draw_my_text("mol: %.2f", mol, 1180, 10);
//     bar.draw(this->get_mouse_pos());
//     Win::draw();
//     EndTextureMode();
// }

void GlassView::draw(void) const
{
    if (!model->is_visible())
        return;

    draw_my_text("osmo?: %.2f", model->get_osmo(), UI_BORDER * 2 + PEDING, rect.y + PEDING * 2 + BUTTON_HEIGHT);
    MyDraw::text("first", ("glass: " + std::to_string(model->get_id())), UI_BORDER * 2 + PEDING + 200, rect.y + PEDING * 2 + BUTTON_HEIGHT, 40, BLACK);

    DrawText(model->get_warning().c_str(), PEDING * 3 + 50, LINE + 170, 30, RED);
    bar.draw(this->get_mouse_pos());
    Win::draw();
}