/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Game.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kvanden- <kvanden-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/13 18:56:03 by kvanden-          #+#    #+#             */
/*   Updated: 2025/05/20 20:47:41 by kvanden-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Game.hpp"
#include "Settings.hpp"

Game::Game() : App("tiboon", 0, 0, 60)
{
while (!IsWindowReady()) {
}
    rect.height = 80;
    rect.width = (GetScreenWidth() * 2 / 3) - 20;
    rect.x = 10;
    rect.y = 10;
    load_data(data);
    win.add_ui(std::make_unique<TextInp>(
        PEDING * 2, PEDING * 2, 100, 50,
        [this](UI &ui)
        { create_new_player(ui); }, "name?"));
}

Game::~Game()
{
}

void Game::create_new_player(UI &ui)
{
    sim.set_rect(); // //////////////////////
    TextInp &t = dynamic_cast<TextInp &>(ui);

    auto tokel = std::make_unique<Tokel>(t.get_rect().x, t.get_rect().y, 100, 50, t.get_text(),
                                         [this](UI &uii)
                                         { switch_players(uii); });
    Tokel *tokel_ptr = tokel.get();

    auto pl = std::make_unique<Player>(t.get_text(), data, sim);
    Player *pl_ptr = pl.get();
    activePlayer = pl_ptr;

    win.add_ui(std::move(tokel));
    win.add_ui(std::move(pl));

    players.push_back({tokel_ptr, pl_ptr});

    t.get_text().clear();
    t.move(120, 0);
    tokel_ptr->set_tokel(true);
    sim.reset();
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
            activePlayer->fiks_sim();
        }
    }
}

void Game::draw() const
{
    ClearBackground(COL_BG);
    DrawRectangleRounded(rect, 0.2, 8, COL_1);
    DrawRectangleRoundedLinesEx(rect, 0.2, 8, 6.0f, BLACK);
    win.draw();
    sim.draw();
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
    this->sim.update(GetFrameTime());
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