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

Player::Player(std::string &name, GameData &data, Sim &sim) : 
name(name), data(data), sim(sim),
glass(Glass(data, [this](UI &ui){ next_glass(ui); }, sim))
{
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
    glass.update();
}

void Player::draw(void) const
{
    if (!_is_visible)
        return;

    DrawText(("Code: " + (code.length() ? code : "None")).c_str(), 50, 100, 80, BLACK);
    DrawText(("Name: " + name).c_str(), 600, 100, 80, BLACK);
    
    history.draw();
    glass.draw();
}
