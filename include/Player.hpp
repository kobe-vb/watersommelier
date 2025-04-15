/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Player.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kvanden- <kvanden-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/24 11:24:37 by kvanden-          #+#    #+#             */
/*   Updated: 2025/03/24 11:29:13 by kvanden-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# pragma once

#include "Win.hpp"
#include "Glass.hpp"

class Player: public Win
{
private:
    const std::string name;
public:
    Player(std::string &name);
    ~Player() = default;;
};

