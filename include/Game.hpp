/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Game.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kvanden- <kvanden-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/13 18:55:01 by kvanden-          #+#    #+#             */
/*   Updated: 2025/05/15 17:33:04 by kvanden-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

# include "App.hpp"
# include "Win.hpp"
# include "Player.hpp"
# include "TextInp.hpp"
# include "GameData.hpp"
# include "Sim.hpp"
# include "BarcodeReader.hpp"

# include <vector>

class Game : public App
{
    private:

        BarcodeReader barcode_reader;
        std::vector<Player_data> players;
        GameData data;
        Win win;
        Sim sim;
        
        Player *activePlayer = nullptr;
        
        Rectangle rect;
                
    private:  
        void handleCode(const std::string &code);
        bool name_is_taken(const std::string &name);
        void save_data();

    public:
        Game();
        ~Game();
        
        void update() override;
        void draw() const override;
        void create_new_player(UI &ui);
        void switch_players(UI &ui);
    };

