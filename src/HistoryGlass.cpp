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

void draw_my_text(const char *name, float val, int x, int y)
{
    char buffer[64];
    snprintf(buffer, sizeof(buffer), name, val);
    DrawText(buffer, x, y, 30, DARKBLUE);
}

HistoryGlass::HistoryGlass(int i, Glass &glass) : BufferedWin((GetScreenWidth() * 1 / 3) + PEDING, LINE + PEDING * 4,
                                                              (GetScreenWidth() * 1 / 3) - PEDING * 4, 120),
                                                  i(i),
                                                  bar(glass.bar),
                                                  comment(glass.get_comment()),
                                                  ph(glass.ph), mol(glass.mol)
{
    bar.set_pos(0, 0);
    rect = {((float)GetScreenWidth() * 1 / 3) + PEDING, LINE + PEDING * 4, (float)win.texture.width, (float)win.texture.height};

    std::regex hashtagRegex("#\\w+");
    std::smatch matches;

    auto it = std::sregex_iterator(comment.begin(), comment.end(), hashtagRegex);
    auto end = std::sregex_iterator();

    for (; it != end; ++it)
        keyWords += it->str() + " ";

    if (!keyWords.empty())
        keyWords.pop_back();
};

void HistoryGlass::save_data(std::ofstream &file)
{
    file << comment << ", final comment i" << "," << ph << "," << mol;

    auto elements = bar.get_data();

    std::ifstream f("data/input/ions.txt");
    std::string line;

    if (f.is_open())
    {
        std::getline(f, line); // lees header regel

        std::stringstream ss(line);
        std::string token;

        std::vector<std::string> header;

        // Split header in tokens
        while (std::getline(ss, token, ','))
        {
            // trim spaties links en rechts
            if (!token.empty() && token[0] == ' ')
                token.erase(0, token.find_first_not_of(" "));
            if (!token.empty() && token.back() == ' ')
                token.erase(token.find_last_not_of(" ") + 1);

            header.push_back(token);
        }

        // Nu over de header tokens loopen
        for (const auto &col : header)
        {
            bool found = false;
            for (auto &element : elements)
            {
                if (element.name == col)
                {
                    file << element.val;
                    found = true;
                    element.val = 0;
                    break;
                }
            }
            if (!found)
                file << 0;

            file << ","; // kolomscheiding
        }

        f.close();
    }
    std::vector<std::string> list;
    for (auto &&part : keyWords | std::views::split(' '))
        list.emplace_back(part.begin(), part.end());

    file << "#" << list.size() << ",";
    for (auto &&word : list)
        file << word << ",score,";
    file << std::endl;

    /// debug

    std::cout << "nog toe te voegen: ";
    for (auto &&element : elements)
    {
        if (element.val != 0)
            std::cout << element.name << ",";
    }
    std::cout << std::endl;

}

std::string HistoryGlass::to_json() const
{
    std::stringstream json;
    json << "    {\n";
    json << "      \"ph\": " << ph << ",\n";
    json << "      \"mol\": " << mol << ",\n";
    json << "      \"comment\": \"" << comment << "\",\n";
    json << "      \"keywords\": \"" << keyWords << "\"\n";
    json << "    }";
    // TODO: save elements
    return json.str();
}

void HistoryGlass::set_pos(int i, float scrollOffset)
{
    this->pos.x = (GetScreenWidth() * 1 / 3) + PEDING;
    this->pos.y = LINE + PEDING * 4 + (i * 140) - scrollOffset;

    rect.x = pos.x;
    rect.y = pos.y;

    float screenTop = LINE;
    float screenBottom = GetScreenHeight() - PEDING;

    if (rect.y + rect.height < screenTop || rect.y > screenBottom)
    {
        this->_is_visible = false;
    }
    else
    {
        this->_is_visible = true;
    }

    this->i++; // compiler
}

void HistoryGlass::draw(void) const
{
    if (!_is_visible)
        return;

    DrawRectangleRounded(rect, ROUNDED, 10, COL_1);
    DrawRectangleRoundedLinesEx(rect, ROUNDED, 10, 6.0f, BLACK);

    DrawTextureRec(
        this->win.texture,
        (Rectangle){0, 0, (float)this->win.texture.width, -(float)this->win.texture.height},
        this->pos,
        WHITE);
};

void HistoryGlass::draww(void) const
{
    if (!_is_visible)
        return;

    BeginTextureMode(this->win);
    clear();

    draw_my_text("pH: %.2f", ph, PEDING * 3, 80);
    draw_my_text("mol: %.2f", mol, PEDING * 3 + 200, 80);
    DrawText(keyWords.c_str(), PEDING * 3, 100, 20, DARKBLUE);

    bar.draw(this->get_mouse_pos());
    EndTextureMode();
};
