/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Player.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kvanden- <kvanden-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/24 11:29:17 by kvanden-          #+#    #+#             */
/*   Updated: 2025/05/14 16:36:26 by kvanden-         ###   ########.fr       */
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
    
    for (int i = 0; i < get_num_of_elements(); i++)
        dynamic_cast<Glass &>(*get_ui_at(i)).draww();
    BeginScissorMode(0, 200, 2000, 800);
    Win::draw();
    EndScissorMode();
}
