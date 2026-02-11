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

# include "ActivityTracker.hpp"

# include "PauseView.hpp"

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

        PauseView pause_view;

        Win win;

        Rectangle screen_rect;

        Music music;

        std::vector<Player_data> players;
        TextInpView *nameInput;

        bool isIdle;
        ActivityTracker activity;

        void create_new_player(void);
                
    public:
        GameView();
        ~GameView();
        
        void update() override;
        void draw() const override;
    };

