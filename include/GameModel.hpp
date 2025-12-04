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
# include "PlayerModel.hpp"
# include "GameData.hpp"
# include "Sim.hpp"
# include "BarcodeReader.hpp"
#include "TextInpModel.hpp"

# include <vector>

class GameModel
{
    private:

        BarcodeReader barcode_reader;
        std::vector<std::unique_ptr<PlayerModel>> players;
        std::vector<TokelModel> buttons;
        GameData data;
        Sim sim;
        
        PlayerModel *activePlayer = nullptr;

        TextInpModel name_input = TextInpModel("type name", [this]() {create_player();});
                        
    private:  
        void handleCode(const std::string &code);
        
        public:
        GameModel();
        ~GameModel() = default;
        
        bool update(void);
        int create_player(void);
        void switch_players(int id);
        void save_data();
        bool name_is_taken(const std::string &name);
        void reset_sim(void);
        
        TextInpModel &get_name_input(void) { return name_input; }
        PlayerModel *get_player(int id) { return players[id].get(); }
        TokelModel &get_tokel(int id) { return buttons[id]; }
    };

