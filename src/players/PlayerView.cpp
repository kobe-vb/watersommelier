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

PlayerView::PlayerView(PlayerModel *model) : model(model), glass(GlassView(&model->get_glass(), rect))
{
}


bool PlayerView::capture_tab(int direction)
{
    if (!_is_active)
        return (false);
    return (glass.capture_tab(direction));
}


bool PlayerView::update(void)
{
    if (!_is_active)
        return (false);

    Win::update();
    model->update();
    return (glass.update());
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
    if (!_is_visible)
        return;

    MyDraw::text("first", ("Code: " + (model->get_code().length() ? model->get_code() : "None")).c_str(), UI_BORDER * 2 + PEDING, UI_BORDER + PEDING + BUTTON_HEIGHT, 80, WHITE);
    MyDraw::text("first", ("Name: " + model->get_name()).c_str(), UI_BORDER * 2 + PEDING, UI_BORDER + PEDING + BUTTON_HEIGHT + 70, 80, WHITE);

    // DrawText(("Code: " + (code.length() ? code : "None")).c_str(), 50, 200, 80, BLACK);
    // DrawText(("Name: " + name).c_str(), 600, 200, 80, BLACK);

    // history.draw();

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
    Win::draw();
}
