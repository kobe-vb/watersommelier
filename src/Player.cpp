/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Player.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kvanden- <kvanden-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/24 11:29:17 by kvanden-          #+#    #+#             */
/*   Updated: 2025/03/24 11:29:24 by kvanden-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "Player.hpp"

Player::Player(std::string &name, GameData &data) : name(name)
{
    add_ui(std::make_unique<Glass>(0, data));
}
