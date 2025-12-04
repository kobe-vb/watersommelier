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
# include "PlayerView.hpp"
# include "TextInpView.hpp"
# include "TextInpModel.hpp"
# include "GameData.hpp"
# include "Sim.hpp"
# include "BarcodeReader.hpp"

#include "GameModel.hpp"

# include "TokelModel.hpp"
# include "TokelView.hpp"

# include <vector>

struct Player_data
{
    TokelView *tokel;
    PlayerView *player;
};

class GameView : public App
{
    private:
        GameModel model;

        Win win;

        Rectangle players_rect;
        Rectangle screen_rect;

        Music music;

        std::vector<Player_data> players;
        TextInpView *nameInput;

        void create_new_player(void);
        void switch_players(int new_id);
                
    public:
        GameView();
        ~GameView();
        
        void update() override;
        void draw() const override;
    };

