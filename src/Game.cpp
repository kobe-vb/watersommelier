/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Game.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kvanden- <kvanden-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/13 18:56:03 by kvanden-          #+#    #+#             */
/*   Updated: 2025/05/15 17:55:37 by kvanden-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Game.hpp"

Game::Game() : App("tiboon", 1500, 600, 60)
{

    load_data(data);
    win.add_ui(std::make_unique<TextInp>(
        10, 10, 100, 50,
        [this](UI &ui)
        { create_new_player(ui); }, "name?"));
}

Game::~Game()
{
}

void Game::create_new_player(UI &ui)
{
    TextInp &t = dynamic_cast<TextInp &>(ui);

    auto tokel = std::make_unique<Tokel>(t.get_rect().x, 10, 100, 50, t.get_text(),
                                         [this](UI &uii)
                                         { switch_players(uii); });
    Tokel *tokel_ptr = tokel.get();

    auto pl = std::make_unique<Player>(t.get_text(), data);
    Player *pl_ptr = pl.get();
    activePlayer = pl_ptr;

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
            pl.player->disable();
        }
        else
        {
            pl.player->activate();
            activePlayer = pl.player;
        }
    }
}

void Game::draw() const
{
    ClearBackground(RAYWHITE);
    win.draw();
    // sim.draw();
}

void Game::handleCode()
{
    std::cout << "code: " << this->code << std::endl;

    for (auto& playerWrapper : players)
    {
        if (playerWrapper.player->is_my_code(this->code))
        {
            playerWrapper.tokel->set_tokel(true);
            return;
        }
    }
    if (this->activePlayer && this->activePlayer->take_code(this->code))
        return;
}


void Game::update()
{
    // this->sim.update(GetFrameTime());
    this->win.update();
    this->win.update_tabs();

    this->code.clear();
    int key = GetCharPressed();
    while (key > 0)
    {
        this->code += (char)key;
        key = GetCharPressed();
    }
    if (this->code.length() > 2)
        this->handleCode();
}