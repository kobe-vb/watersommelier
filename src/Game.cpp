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

#include <fstream>
#include <cstdint>

#include "Game.hpp"
#include "Settings.hpp"
#include "SudoPlayer.hpp"
#include <WebsitePlayer.hpp>

static void saveCounter(size_t counter, const std::string& filename)
{
    std::ofstream out(filename, std::ios::binary);
    if (!out) {
        std::cerr << "Failed to open file for writing: " << filename << std::endl;
        return;
    }
    out.write(reinterpret_cast<const char*>(&counter), sizeof(counter));
}

static size_t loadCounter(const std::string& filename)
{
    std::ifstream in(filename, std::ios::binary);
    if (!in) 
        return 1;
    size_t counter;
    in.read(reinterpret_cast<char*>(&counter), sizeof(counter));
    return counter;
}

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
        PEDING * 2, PEDING * 2, 100, 60,
        [this](UI &ui)
        { create_new_player(ui); }, "name?"));
}

Game::~Game()
{
    save_data();
}

void Game::save_data(void)
{
    size_t counter = loadCounter("data/output/counter.bin");
    std::ofstream file("data/output/data.csv", std::ios::app);
    if (file.is_open())
    {
        for (auto &player : players)
            player.player->save_data(file, counter);
        file.close();
    }
    saveCounter(counter, "data/output/counter.bin");
}

void Game::create_new_player(UI &ui)
{
    sim.set_rect(); // //////////////////////
    TextInp &t = dynamic_cast<TextInp &>(ui);

    auto tokel = std::make_unique<Tokel>(t.get_rect().x, t.get_rect().y, 110, 60, t.get_text(),
                                         [this](UI &uii)
                                         { switch_players(uii); });
    Tokel *tokel_ptr = tokel.get();

    Player *pl_ptr = nullptr;
    if (t.get_text() == "tiboon")
    {
        auto pl = std::make_unique<SudoPlayer>(t.get_text(), data, sim, players);
        pl_ptr = pl.get();
        win.add_ui(std::move(pl));
    }
    else if (t.get_text() == "web")
    {
        auto pl = std::make_unique<WebPlayer>(t.get_text(), data, sim, players);
        pl_ptr = pl.get();
        win.add_ui(std::move(pl));
    }
    else
    {
        auto pl = std::make_unique<Player>(t.get_text(), data, sim);
        if (t.get_text() == "demo")
            pl->demo();
        pl_ptr = pl.get();
        win.add_ui(std::move(pl));
    }
    activePlayer = pl_ptr;

    win.add_ui(std::move(tokel));

    players.push_back({tokel_ptr, pl_ptr});

    t.get_text().clear();
    t.move(125, 0);
    tokel_ptr->set_tokel(true);
    sim.reset();

    if (players.size() == 9 )
    {
        t.set_text("tiboon");
        t.run_callback();
        return;
    }

    if (players.size() == 10 || this->name_is_taken("tiboon"))
        win.pop_ui_front();
}

bool Game::name_is_taken(const std::string &name)
{
    for (auto &player : players)
    {
        if (player.player->get_name() == name)
            return (true);
    }
    return (false);
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
    sim.draw();
    win.draw();
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