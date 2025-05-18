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

# include "Player.hpp"

// BufferedWin(100, 100, 1800, 600),
Player::Player(std::string &name, GameData &data) : 
name(name), data(data),
scroll(ScrollBar({1800, 100, 70, 500}, 800, 200, [this](float height) { scroll_update(height); }))
{
    add_ui(std::make_unique<Glass>(0, 200, data, [this](UI &ui)
    { next_glass(ui); }));
}

void Player::next_glass(UI &ui)
{
    dynamic_cast<TextInp &>(ui).remove_active();
    int i = get_num_of_elements();
    
    get_ui_at(i - 1)->set_active(false);
    float y = dynamic_cast<Glass *>(get_ui_at(i - 1))->get_pos().y + 200;
    add_ui(std::make_unique<Glass>(i, y, data, [this](UI &ui)
    { next_glass(ui); }));
    scroll.add_height(300);
    scroll.scroll(300);
}

void Player::scroll_update(float height)
{
    std::cout << "Scroll update: " << height << std::endl;
    for (int i = 0; i < get_num_of_elements(); i++)
        dynamic_cast<Glass &>(*get_ui_at(i)).move(0, -height);
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
    scroll.update();
    Win::update();
}

void Player::draw(void) const
{
    if (!_is_visible)
        return;
    scroll.draw();

    DrawText(("code: " + code).c_str(), 50, 100, 20, BLACK);
    
    for (int i = 0; i < get_num_of_elements(); i++)
        dynamic_cast<Glass &>(*get_ui_at(i)).draww();
    BeginScissorMode(0, 200, 2000, 800);
    Win::draw();
    EndScissorMode();
}
