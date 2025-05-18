/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Player.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kvanden- <kvanden-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/24 11:24:37 by kvanden-          #+#    #+#             */
/*   Updated: 2025/05/15 16:23:25 by kvanden-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# pragma once

#include "BufferedWin.hpp"
#include "Glass.hpp"
#include "GameData.hpp"
#include "ScrollBar.hpp"

class Player: public Win
{
private:
    const std::string name;
    GameData &data;
    ScrollBar scroll;
    std::string code;
    
public:
    Player(std::string &name, GameData &data);
    ~Player() = default;

    void next_glass(UI &ui);
    void scroll_update(float height);

    bool take_code(std::string &code);
    bool is_my_code(std::string &code) const;

    void update(void) override;
    void draw(void) const override;
};

