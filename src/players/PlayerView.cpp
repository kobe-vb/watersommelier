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

    Win::update();
    model->update();
    return (glass.update() || score_glass.update() || website.update());
}

/*
void PlayerView::draw_qr(void) const
{

    if (!qr.has_value())
        return;

    const int pixelSize = 10;
    const int qrSize = qr.value().getSize();

    int dx = rect.x + 150;
    int dy = rect.y + 50;

    for (int y = 0; y < qrSize; y++)
    {
        for (int x = 0; x < qrSize; x++)
        {
            if (qr.value().getModule(x, y))
            {
                DrawRectangle(dx + x * pixelSize, dy + y * pixelSize, pixelSize, pixelSize, BLACK);
            }
        }
    }

    DrawText(("Website: " + website).c_str(), 50, rect.y + 400, 30, BLACK);
}
    */

void PlayerView::draw(void) const
{
    if (!model->is_visible())
        return;

    DrawRectangleRounded(players_info_rect, ROUNDED * 3, 8, UIView::get_dcolor(UiColors::FIRST));
    if (BORDER_WIDTH > 0)
        DrawRectangleRoundedLinesEx(players_info_rect, ROUNDED, 8, BORDER_WIDTH, UIView::get_dcolor(UiColors::BORDER));

    MyDraw::text("first", ("Code: " + (model->get_code().length() ? model->get_code() : "None")).c_str(), UI_BORDER * 2 + PEDING, UI_BORDER + PEDING + BUTTON_HEIGHT, 80, WHITE);
    MyDraw::text("first", ("Name: " + model->get_name()).c_str(), UI_BORDER * 2 + PEDING, UI_BORDER + PEDING + BUTTON_HEIGHT + 70, 80, WHITE);

    // DrawText(("Code: " + (code.length() ? code : "None")).c_str(), 50, 200, 80, BLACK);
    // DrawText(("Name: " + name).c_str(), 600, 200, 80, BLACK);

    model->history.draw();

    DrawRectangleRounded(rect, ROUNDED, 10, get_color(UiColors::FIRST));
    if (BORDER_WIDTH > 0)
        DrawRectangleRoundedLinesEx(rect, ROUNDED, 10, BORDER_WIDTH, get_color(UiColors::BORDER));

    // if (!website_data.empty())
    // {
    //     DrawText("nice i got the data", 50, rect.y + 300, 30, BLACK);
    //     Win::draw();
    //     return;
    // }

    // if (!website.empty())
    // {
    //     draw_qr();
    //     Win::draw();
    //     return;
    // }
    glass.draw();
    score_glass.draw();
    website.draw();

    Win::draw();
}
