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
#include <filesystem>

#include "Game.hpp"
#include "Settings.hpp"
#include "SudoPlayer.hpp"
#include "MyDraw.hpp"
#include <WebsitePlayer.hpp>

static void saveCounter(size_t counter, const std::string &filename)
{
    std::ofstream out(filename, std::ios::binary);
    if (!out)
    {
        std::cerr << "Failed to open file for writing: " << filename << std::endl;
        return;
    }
    out.write(reinterpret_cast<const char *>(&counter), sizeof(counter));
}

static size_t loadCounter(const std::string &filename)
{
    std::ifstream in(filename, std::ios::binary);
    if (!in)
        return 1;
    size_t counter;
    in.read(reinterpret_cast<char *>(&counter), sizeof(counter));
    return counter;
}

Game::Game() : App("pompion", 0, 0, 60)
{
    while (!IsWindowReady())
    {
    }
    InitAudioDevice();
    music = LoadMusicStream("data/sounds/music.mp3");
    MyDraw::init("data/fonts/");

    barcode_reader = BarcodeReader([this](const std::string &code)
                                   { handleCode(code); });

    screen_rect.x = UI_BORDER;
    screen_rect.y = UI_BORDER;
    screen_rect.width = GetScreenWidth() - (UI_BORDER * 2);
    screen_rect.height = GetScreenHeight() - (UI_BORDER * 2);

    players_rect.x = UI_BORDER * 2;
    players_rect.y = UI_BORDER + PLAYER_HEIGHT + PEDING * 2;
    players_rect.width = GetScreenWidth() - (UI_BORDER * 4);
    players_rect.height = LINE - players_rect.y - PEDING;

    load_data(data);
    win.add_ui(std::make_unique<TextInp>(
        UI_BORDER * 2, UI_BORDER + PEDING, PLAYER_WIDTH, PLAYER_HEIGHT,
        [this](UI &ui)
        { create_new_player(ui); }, "name?"));
}

Game::~Game()
{
    UnloadMusicStream(music);
    CloseAudioDevice();
    save_data();
}

void Game::save_data(void)
{
    size_t counter = loadCounter("data/output/counter.bin");

    bool file_exists = std::filesystem::exists("data/output/data.csv");
    std::ofstream file("data/output/data.csv", std::ios::app);
    if (file.is_open())
    {
        if (!file_exists)
        {
            file << "i;datum;name;SsC Zoet; SsC Zout; SsC zuur; SsC Bitter; SsC Umami;comment;end comment;osmo;Tot volume(ml);";
            for (auto &ion : data.ions)
                file << ion.first << " %" << ";" << ion.first << " mol" << ";" << ion.first << " mg" << ";";
            file << "hastags;" << std::endl;
        }

        for (auto &player : players)
        {
            if (!DEBUG && (player.player->get_name().starts_with("demo") || player.player->get_name().starts_with("web")))
                continue;
            player.player->save_data(file, counter, data);
        }
        file.close();
    }
    saveCounter(counter, "data/output/counter.bin");
}

void Game::create_new_player(UI &ui)
{

    TextInp &t = dynamic_cast<TextInp &>(ui);

    if ((name_is_taken(t.get_text()) || t.get_text().empty()) && !DEBUG)
    {
        t.set_color(UiColors::BG, RED);
        return;
    }
    t.clear_color(UiColors::BG);

    sim.set_rect(); // //////////////////////
    sim.reset();

    auto tokel = std::make_unique<Tokel>(t.get_rect().x, t.get_rect().y, t.get_rect().width, t.get_rect().height, t.get_text(),
                                         [this](UI &uii)
                                         { switch_players(uii); });
    Tokel *tokel_ptr = tokel.get();
    win.add_ui(std::move(tokel));

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

    players.push_back({tokel_ptr, pl_ptr});

    t.get_text().clear();
    t.move(PLAYER_WIDTH + PLAYER_PEDING, 0);
    tokel_ptr->set_tokel(true);

    if (players.size() == 9)
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
    ClearBackground(UI::get_dcolor(UiColors::FIRST));

    DrawRectangleRounded(screen_rect, 0.05, 8, UI::get_dcolor(UiColors::BG));
    if (BORDER_WIDTH > 0)
        DrawRectangleRoundedLinesEx(screen_rect, 0.05, 8, BORDER_WIDTH, UI::get_dcolor(UiColors::BORDER));

    DrawRectangleRounded(players_rect, 0.05, 8, UI::get_dcolor(UiColors::FIRST));
    if (BORDER_WIDTH > 0)
        DrawRectangleRoundedLinesEx(players_rect, ROUNDED, 8, BORDER_WIDTH, UI::get_dcolor(UiColors::BORDER));

    sim.draw();
    win.draw();
    if (DEBUG)
        DrawFPS(UI_BORDER * 2, 5);
}

void Game::handleCode(const std::string &rawCode)
{
    // Maak een schoon exemplaar van de code
    std::string code = rawCode;

    // Verwijder alle niet-printbare tekens (zoals STX, ETX, CR, LF)
    code.erase(std::remove_if(code.begin(), code.end(),
                              [](unsigned char c)
                              { return !std::isprint(c); }),
               code.end());

    // Trim spaties aan begin en einde
    code.erase(0, code.find_first_not_of(' '));
    code.erase(code.find_last_not_of(' ') + 1);

    std::cout << "code: \"" << code << "\"" << std::endl;

    if (this->activePlayer && this->activePlayer->take_code_for_dropdown(code))
        return;

    for (auto &playerWrapper : players)
    {
        if (playerWrapper.player->is_my_code(code))
        {
            playerWrapper.tokel->set_tokel(true);
            return;
        }
    }
    if (!this->activePlayer)
        return;
    this->activePlayer->set_code(code);
}

void Game::update()
{
    if (IsKeyDown(KEY_LEFT_CONTROL))
    {
        if (IsKeyPressed(KEY_P))
        {
            if (IsMusicStreamPlaying(music))
                PauseMusicStream(music);
            else
                PlayMusicStream(music);
        }
        if (IsKeyPressed(KEY_DELETE))
            this->stop();
    }

    UpdateMusicStream(music);

    this->sim.update(GetFrameTime());
    barcode_reader.update();
    if (barcode_reader.isBuilding())
        return;
    this->win.update();
    this->win.update_tabs();
}