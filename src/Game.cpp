/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Game.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kvanden- <kvanden-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/13 18:56:03 by kvanden-          #+#    #+#             */
/*   Updated: 2025/05/15 16:30:57 by kvanden-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Game.hpp"

Game::Game() : App("tiboon", 2000, 600, 60)
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

    auto pl = std::make_unique<Player>(t.get_text(), data, nextPlayerCode);
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
}

void Game::prepareNextPlayer()
{
    this->nextPlayerCode = this->code;
    auto& inputField = dynamic_cast<TextInp&>(*this->win.get_ui_at(0));
    inputField.set_active(true);
}

void Game::handleCode()
{
    // Probeer eerst of de actieve speler de code kan gebruiken
    if (this->activePlayer && this->activePlayer->take_code(this->code))
        return;

    // Loop door alle spelers om te controleren of de code bij iemand hoort
    for (auto& playerWrapper : players)
    {
        if (playerWrapper.player->is_my_code(this->code))
        {
            playerWrapper.tokel->set_tokel(true);
            return;
        }
    }
    // Als niemand de code herkent, bereid de volgende speler voor
    this->prepareNextPlayer();
}


void Game::update()
{
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