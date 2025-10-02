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
#include "WebsiteData.hpp"

#include <thread>

Player::Player(std::string &name, GameData &data, Sim &sim) : name(name),
                                                              glass(Glass(data, [this](UI &ui)
                                                                          { next_glass(ui); }, sim, rect))
{
}

void Player::demo(void)
{
    int max = std::rand() % 6 + 4;
    for (int i = 0; i < max; i++)
    {
        glass.generate_random_data();
        history.saveGlass(glass);
        glass.reset();
    }
    glass.generate_random_data(false);
    glass.reset_sim();
}

void Player::save_data(std::ofstream &file, size_t &counter, GameData &data)
{
    if (history.get_num_of_elements() == 0)
        return;
    history.save_data(file, counter, name, data, website_data);
}

std::string Player::to_json() const
{
    std::stringstream json;
    json << "{\n";
    json << "  \"name\": \"" << name << "\",\n";
    json << "  \"glasses\": [\n";

    for (int i = 0; i < history.get_num_of_elements(); i++)
    {
        if (i > 0)
            json << ",\n";
        const HistoryGlass &glass = dynamic_cast<const HistoryGlass &>(*history.get_ui_at(i));
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

    Win::update();

    history.update();
    glass.update();
}

void Player::set_website(std::string website)
{
    this->website = website;
    this->qr = qrcodegen::QrCode::encodeText(website.c_str(), qrcodegen::QrCode::Ecc::LOW);
    // this->qr = qrcodegen::QrCode::encodeText("WIFI:T:WPA;S:yourAP;P:yourPassword;;", qrcodegen::QrCode::Ecc::LOW);

    this->add_ui(std::make_unique<Button>(
        rect.x + 50, rect.y + 500, 200, 50, "Go to website",
        [url = this->website](UI& ui)
        {
            (void)ui;
            std::thread([url]()
            {
                std::string command = "start \"\" \"" + url + "\"";
                system(command.c_str());
            }).detach();
        }));
}

void Player::set_website_data(std::string website_data)
{
    this->website_data = WebsiteData(website_data);
    this->pop_ui_back();
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

    DrawRectangleRounded(rect, ROUNDED, 10, get_color(UiColors::FIRST));
    DrawRectangleRoundedLinesEx(rect, ROUNDED, 10, 6.0f, BLACK);

    if (!website_data.empty())
    {
        DrawText("nice i got the data", 50, rect.y + 300, 30, BLACK);
        Win::draw();
        return;
    }

    if (!website.empty())
    {
        draw_qr();
        Win::draw();
        return;
    }
    glass.draw();
    Win::draw();
}
