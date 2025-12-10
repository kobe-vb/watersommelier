/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HistoryGlass.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kvanden- <kvanden-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/19 11:37:33 by kvanden-          #+#    #+#             */
/*   Updated: 2025/05/19 12:49:13 by kvanden-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "HistoryGlass.hpp"
#include "Settings.hpp"

#include <string>
#include <regex>

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <MyDraw.hpp>

void draw_my_text(const char *name, float mol, int x, int y)
{
    char buffer[64];
    snprintf(buffer, sizeof(buffer), name, mol);
    DrawText(buffer, x, y, 30, DARKBLUE);
}

HistoryGlass::HistoryGlass(int id, Rectangle &ref_rect, GlassModel &glass, ScoreGlassModel &scoreGlass) : BufferedWin(&model, ref_rect.x, ref_rect.y, ref_rect.width * 1.3, 200),
                                                                                    i(id),
                                                                                    bar(glass.bar),
                                                                                    comment(scoreGlass.get_comment_text()),
                                                                                    hastags(scoreGlass.get_hastags()),
                                                                                    osmo(glass.osmo),
                                                                                    volume(glass.volume)
{
    rect.width = ((GetScreenWidth() - (UI_BORDER * 4) - (PEDING * 2)) * 1 / 3);
    rect.height = 180;

    rect.x = PEDING;
    rect.y = PEDING;
    rect.width -= PEDING * 2;

    bar_view = StackedBarHView(&bar, rect.x + PEDING, rect.y + PEDING, (int)(rect.width - PEDING * 2), BUTTON_HEIGHT);
};

void HistoryGlass::save_data(CSVDownloader &csv, GameData &data, WebsiteData &websiteData)
{
    csv << comment << websiteData.get("final_comment") << osmo << volume;

    for (auto &ion : data.ions)
    {
        float mol = 0;
        Data *ion_data = bar[ion.first];
        if (ion_data != nullptr)
            mol = ion_data->val;
        csv << (mol / bar.get_total_volume()) * 100;            // % --> todo .f2
        csv << mol;                                             // mol
        csv << mol * data.get_ion_data(ion.first).gram_per_mol; // mg
    }

    csv << hastags.size() * 2;
    for (auto &tags : hastags)
        csv << tags.first << tags.second;
}

std::string HistoryGlass::to_json() const
{
    std::stringstream json;
    json << "    {\n";
    json << "      \"id\": \"" << i + 1 << "\",\n";
    json << "      \"tags\": [\n";

    size_t i = 0;
    for (auto it = hastags.begin(); it != hastags.end(); ++it, ++i)
    {
        if (i > 0)
            json << ",";
        json << "\"" << it->first << "\"";
    }

    json << "\n      ]\n";
    json << "    }";
    return json.str();
}

void HistoryGlass::set_pos(int i, float scrollOffset)
{
    this->pos.y = LINE + (rect.height + PEDING) * i - scrollOffset;

    // rect.y = PEDING + (rect.height + PEDING) * i;

    // float screenTop = LINE;
    // float screenBottom = GetScreenHeight() - PEDING;

    // if (rect.y + rect.height < screenTop || rect.y > screenBottom)
    //     model.set_visible(false);
    // else
    //     model.set_visible(true);
}

void HistoryGlass::draw(void) const
{
    if (!model.is_visible())
        return;

    DrawTextureRec(
        this->win.texture,
        (Rectangle){0, 0, (float)this->win.texture.width, -(float)this->win.texture.height},
        this->pos,
        WHITE);
};

void HistoryGlass::draww(void) const
{
    if (!model.is_visible())
        return;

    BeginTextureMode(this->win);
    clear();
    // ClearBackground({255, 0, 0, 150});

    DrawRectangleRounded(rect, ROUNDED, 10, UIView::get_dcolor(UiColors::BG));
    // DrawRectangleRoundedLinesEx(rect, ROUNDED, 10, 6.0f, BLACK);


    // draw_my_text("pH: %.2f", ph, PEDING * 3, 80);
    // draw_my_text("mol: %.2f", mol, PEDING * 3 + 200, 80);
    // DrawText(keyWords.c_str(), PEDING * 3, 100, 20, DARKBLUE);

    MyDraw::text("first", "glass:" + std::to_string(i), rect.x + PEDING, rect.y + BUTTON_HEIGHT + PEDING, 40, BLACK);

    bar_view.draw(this->get_mouse_pos());
    EndTextureMode();
};
