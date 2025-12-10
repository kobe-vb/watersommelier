/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Player.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kvanden- <kvanden-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/24 11:29:17 by kvanden-          #+#    #+#             */
/*   Updated: 2025/05/15 16:24:07 by kvanden-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "PlayerView.hpp"
#include "Settings.hpp"
#include "WebsiteData.hpp"

#include <thread>
#include <MyDraw.hpp>

PlayerView::PlayerView(PlayerModel *model) : Win(model), model(model), glass(GlassView(&model->get_glass(), rect)),
                                             score_glass(ScoreGlassView(&model->get_score_glass(), rect)),
                                             website(WebsiteView(&model->get_websiteModel()))
{
    players_info_rect.x = UI_BORDER * 2;
    players_info_rect.y = UI_BORDER + PLAYER_HEIGHT + PEDING * 2;
    players_info_rect.width = GetScreenWidth() - (UI_BORDER * 4);
    players_info_rect.height = LINE - players_info_rect.y - PEDING;
}

bool PlayerView::capture_tab(int direction)
{
    if (model->is_locked())
        return (false);
    return (Win::capture_tab(direction) || glass.capture_tab(direction) || score_glass.capture_tab(direction));
}

bool PlayerView::update(void)
{
    if (model->is_locked())
        return (false);

    if (IsKeyDown(KEY_LEFT_CONTROL))
    {
        if (IsKeyPressed(KEY_R))
            model->remove_code();
    }

    Win::update();
    model->update();
    return (glass.update() || score_glass.update() || website.update());
}

void PlayerView::draw(void) const
{
    if (!model->is_visible())
        return;

    DrawRectangleRounded(players_info_rect, ROUNDED * 3, 8, UIView::get_dcolor(UiColors::FIRST));
    if (BORDER_WIDTH > 0)
        DrawRectangleRoundedLinesEx(players_info_rect, ROUNDED, 8, BORDER_WIDTH, UIView::get_dcolor(UiColors::BORDER));

    MyDraw::text("first", ("Code: " + (model->get_code().length() ? model->get_code() : "None")).c_str(), UI_BORDER * 2 + PEDING, UI_BORDER + PEDING + BUTTON_HEIGHT, 80, WHITE);
    MyDraw::text("first", ("Name: " + model->get_name()).c_str(), UI_BORDER * 2 + PEDING, UI_BORDER + PEDING + BUTTON_HEIGHT + 70, 80, WHITE);

    if (model->thief)
        MyDraw::text("first", ("Guest: " + model->thief->get_name()).c_str(), (UI_BORDER + PEDING) * 2 + rect.width, UI_BORDER + PEDING + BUTTON_HEIGHT, 80, WHITE);

    model->draw_history();

    DrawRectangleRounded(rect, ROUNDED, 10, get_color(UiColors::FIRST));
    if (BORDER_WIDTH > 0)
        DrawRectangleRoundedLinesEx(rect, ROUNDED, 10, BORDER_WIDTH, get_color(UiColors::BORDER));

    glass.draw();
    score_glass.draw();
    website.draw();

    Win::draw();
}
