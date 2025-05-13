/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Player.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kvanden- <kvanden-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/24 11:24:37 by kvanden-          #+#    #+#             */
/*   Updated: 2025/05/13 17:57:33 by kvanden-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# pragma once

#include "Win.hpp"
#include "Glass.hpp"
#include "GameData.hpp"
#include "ScrollBar.hpp"

class Player: public Win
{
private:
    const std::string name;
    GameData &data;
    ScrollBar scroll;
    
public:
    Player(std::string &name, GameData &data);
    ~Player() = default;

    void next_glass(UI &ui);
    void scroll_update(float height);

    void update(void) override;
    void draw(void) const override;
};

