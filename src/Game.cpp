/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Game.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kvanden- <kvanden-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/13 18:56:03 by kvanden-          #+#    #+#             */
/*   Updated: 2025/03/25 18:22:32 by kvanden-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Game.hpp"

Game::Game() : App("tiboon", 800, 600, 60)
{

    load_data(data);
    win.add_ui(std::make_unique<TextInp>(
        10, 10, 100, 50,
        [this](UI& ui) { create_new_player(ui); }, "name?"
    ));
}

Game::~Game()
{
}

void Game::create_new_player(UI &ui)
{
    TextInp &t = dynamic_cast<TextInp &>(ui);

    auto tokel = std::make_unique<Tokel>(t.get_rect().x, 10, 100, 50, t.get_text(), 
    [this](UI& uii) { switch_players(uii); });
    Tokel* tokel_ptr = tokel.get();

    auto pl = std::make_unique<Player>(t.get_text(), data);
    Player* pl_ptr = pl.get();
    
    win.add_ui(std::move(tokel));
    win.add_ui(std::move(pl));

    players.push_back({tokel_ptr, pl_ptr});

    t.get_text().clear();
    t.move(120, 0);
    tokel_ptr->set_tokel(true);
}


void Game::switch_players(UI &ui)
{
    Tokel &t = dynamic_cast<Tokel &>(ui);
    
    for (auto pl : players)
    {
        if (pl.tokel != &t)
        {
            pl.tokel->set_tokel(false);
            pl.player->set_active(false);
        }
        else
            pl.player->set_active(true);
    }
}

void Game::draw() const
{
    ClearBackground(RAYWHITE);
    win.draw();
}

void Game::update()
{
    win.update();
    win.update_tabs();
}