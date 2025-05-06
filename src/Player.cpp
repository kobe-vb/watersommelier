/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Player.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kvanden- <kvanden-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/24 11:29:17 by kvanden-          #+#    #+#             */
/*   Updated: 2025/05/06 18:30:42 by kvanden-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "Player.hpp"

Player::Player(std::string &name, GameData &data) : name(name), data(data)
{
    add_ui(std::make_unique<Glass>(0, data, [this](UI &ui)
    { next_glass(ui); }));
}

void Player::next_glass(UI &ui)
{
    dynamic_cast<TextInp &>(ui).remove_active();
    int i = get_num_of_elements();
    
    get_ui_at(i - 1)->set_active(false);
    add_ui(std::make_unique<Glass>(get_num_of_elements(), data, [this](UI &ui)
    { next_glass(ui); }));
}