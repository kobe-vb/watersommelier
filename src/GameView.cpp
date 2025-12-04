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

#include "GameView.hpp"
#include "Settings.hpp"
// #include "SudoPlayer.hpp"
#include "MyDraw.hpp"
// #include <WebsitePlayer.hpp>

GameView::GameView() : App("pompion", 0, 0, 60)
{
    while (!IsWindowReady())
    {
    }
    InitAudioDevice();
    music = LoadMusicStream("data/sounds/music.mp3");
    MyDraw::init("data/fonts/");

    screen_rect.x = UI_BORDER;
    screen_rect.y = UI_BORDER;
    screen_rect.width = GetScreenWidth() - (UI_BORDER * 2);
    screen_rect.height = GetScreenHeight() - (UI_BORDER * 2);

    players_rect.x = UI_BORDER * 2;
    players_rect.y = UI_BORDER + PLAYER_HEIGHT + PEDING * 2;
    players_rect.width = GetScreenWidth() - (UI_BORDER * 4);
    players_rect.height = LINE - players_rect.y - PEDING;

    win.add_ui(std::make_unique<TextInpView>(
        &model.get_name_input(),
        UI_BORDER * 2, UI_BORDER + PEDING,
        PLAYER_WIDTH, PLAYER_HEIGHT));
    nameInput = static_cast<TextInpView *>(win.get_last_ui());

    model.get_name_input().set_callback([this]()
                                        { create_new_player(); });
}

GameView::~GameView()
{
    UnloadMusicStream(music);
    CloseAudioDevice();

    model.save_data();
}

void GameView::draw() const
{
    ClearBackground(UI::get_dcolor(UiColors::FIRST));

    DrawRectangleRounded(screen_rect, 0.05, 8, UI::get_dcolor(UiColors::BG));
    if (BORDER_WIDTH > 0)
        DrawRectangleRoundedLinesEx(screen_rect, 0.05, 8, BORDER_WIDTH, UI::get_dcolor(UiColors::BORDER));

    DrawRectangleRounded(players_rect, ROUNDED * 3, 8, UI::get_dcolor(UiColors::FIRST));
    if (BORDER_WIDTH > 0)
        DrawRectangleRoundedLinesEx(players_rect, ROUNDED, 8, BORDER_WIDTH, UI::get_dcolor(UiColors::BORDER));

    // sim.draw(); TODO
    win.draw();
    if (DEBUG)
        DrawFPS(UI_BORDER * 2, 5);
}

// TODO: add other players

// Player *pl_ptr = nullptr;
// if (t.get_text() == "tiboon")
// {
//     auto pl = std::make_unique<SudoPlayer>(t.get_text(), data, sim, players);
//     pl_ptr = pl.get();
//     win.add_ui(std::move(pl));
// }
// else if (t.get_text() == "web")
// {
//     auto pl = std::make_unique<WebPlayer>(t.get_text(), data, sim, players);
//     pl_ptr = pl.get();
//     win.add_ui(std::move(pl));
// }
// else
// {
//     auto pl = std::make_unique<Player>(t.get_text(), data, sim);
//     if (t.get_text() == "demo")
//         pl->demo();
//     pl_ptr = pl.get();
//     win.add_ui(std::move(pl));
// }
void GameView::create_new_player(void)
{
    const std::string &name = model.get_name_input().get_text();

    // check if name is taken
    if (!DEBUG && (model.name_is_taken(name) || name.empty()))
    {
        nameInput->set_color(UiColors::BG, RED);
        return;
    }
    nameInput->clear_color(UiColors::BG);

    model.reset_sim();

    int ind = model.create_player();

    auto pl = std::make_unique<PlayerView>(model.get_player(ind));
    if (name == "demo")
        model.get_player(ind)->demo();

    const auto &rect = nameInput->get_rect();
    auto tokel = std::make_unique<TokelView>(&model.get_tokel(ind), rect.x, rect.y, rect.width, rect.height);

    players.push_back({tokel.get(), pl.get()});
    
    win.add_ui(std::move(tokel));
    win.add_ui(std::move(pl));


    model.get_name_input().reset();
    nameInput->move(PLAYER_WIDTH + PLAYER_PEDING, 0);
    model.get_tokel(ind).set_tokel(true);

    if (players.size() == 9)
    {
        model.get_name_input().set_text("tiboon");
        model.get_name_input().run_callback();
        return;
    }

    if (players.size() == 10 || model.name_is_taken("tiboon"))
        win.pop_ui_front();
}

// void GameView::switch_players(int new_id)
// {
//     int id = 0;
//     for (auto pl : players)
//     {
//         if (id != new_id)
//             pl.tokel->set_tokel(false);
//         else
//             pl.player->activate();
//     }
// }

void GameView::update()
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

    if (!model.update())
        return;
    this->win.update();
    this->win.update_tabs();
}