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

#include "Player.hpp"
#include "Settings.hpp"

Player::Player(std::string &name, GameData &data, Sim &sim) : name(name),
                                                              glass(Glass(data, [this](UI &ui)
                                                                          { next_glass(ui); }, sim, rect))
{
}

void Player::save_data(std::ofstream &file, size_t &counter)
{
    if (history.get_num_of_elements() == 0)
        return;
    size_t beginCounter = counter;
    history.save_data(file, counter, name);
    file << name << " " << beginCounter << "-" << counter - 1
         << ",datum," << name << ", end?" << std::endl;
}

std::string Player::to_json() const
{
    std::stringstream json;
    json << "{\n";
    json << "  \"name\": \"" << name << "\",\n";
    json << "  \"code\": \"" << code << "\",\n";
    json << "  \"history\": [\n";
    
    for (int i = 0; i < history.get_num_of_elements(); i++)
    {
        if (i > 0) json << ",\n";
        const HistoryGlass& glass = dynamic_cast<const HistoryGlass&>(*history.get_ui_at(i));
        json << glass.to_json();
    }
    
    json << "\n  ]\n}";
    return json.str();
}

void Player::fiks_sim(void)
{
    glass.reset_sim();
}

bool Player::capture_tab(void)
{
    if (!_is_active)
        return (false);
    return (glass.capture_tab());
}

void Player::next_glass(UI &ui)
{
    (void)ui;
    history.saveGlass(glass);
    glass.reset();
}

bool Player::is_my_code(std::string &code) const
{
    return (code == this->code);
}

bool Player::take_code(std::string &new_code)
{
    std::cout << "code: " << code.length() << std::endl;
    if (code.length() == 0)
    {
        this->code = new_code;
        return true;
    }
    Glass &g = dynamic_cast<Glass &>(*get_ui_at(0));
    return (g.take_code(new_code));
}

void Player::update(void)
{
    if (!_is_active)
        return;

    history.update();
    glass.update();
}

void Player::set_website(std::string website)
{
    this->website = website;
    this->qr = qrcodegen::QrCode::encodeText(website.c_str(), qrcodegen::QrCode::Ecc::LOW);
}

void Player::draw_qr(void) const
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

void Player::draw(void) const
{
    if (!_is_visible)
        return;

    DrawText(("Code: " + (code.length() ? code : "None")).c_str(), 50, 100, 80, BLACK);
    DrawText(("Name: " + name).c_str(), 600, 100, 80, BLACK);

    history.draw();

    DrawRectangleRounded(rect, ROUNDED, 10, COL_1);
    DrawRectangleRoundedLinesEx(rect, ROUNDED, 10, 6.0f, BLACK);

    if (!website.empty())
    {
        draw_qr();
        return;
    }
    glass.draw();
}
