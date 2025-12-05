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
#include <SudoPlayerModel.hpp>
#include <SudoPlayerView.hpp>
// #include <WebsitePlayer.hpp>

GameView::GameView() : App("pompion", 0, 0, 60), win(Win(&model))
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
    ClearBackground(UIView::get_dcolor(UiColors::FIRST));

    DrawRectangleRounded(screen_rect, 0.05, 8, UIView::get_dcolor(UiColors::BG));
    if (BORDER_WIDTH > 0)
        DrawRectangleRoundedLinesEx(screen_rect, 0.05, 8, BORDER_WIDTH, UIView::get_dcolor(UiColors::BORDER));

    model.get_sim().draw();
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
    model.get_tokel(ind).set_callback([this, ind]()
                                      { switch_players(ind); });

    PlayerModel *pm = model.get_player(ind);
    std::unique_ptr<PlayerView> pl;
    if (pm->role == Role::Sudo)
        pl = std::make_unique<SudoPlayerView>(model.get_player(ind));
    else
        pl = std::make_unique<PlayerView>(model.get_player(ind));

    if (name == "demo")
        model.get_player(ind)->demo();

    const auto &rect = nameInput->get_rect();
    auto tokel = std::make_unique<TokelView>(&model.get_tokel(ind), rect.x, rect.y, rect.width, rect.height);

    players.push_back({tokel.get(), pl.get()});

    win.add_ui(std::move(tokel));
    win.add_ui(std::move(pl));

    model.get_name_input().reset();
    nameInput->move(PLAYER_WIDTH + PLAYER_PEDING, 0);
    switch_players(ind);

    if (players.size() == 9)
    {
        model.get_name_input().set_text("tiboon");
        model.get_name_input().run_callback();
        return;
    }

    if (players.size() == 10 || model.name_is_taken("tiboon"))
        win.pop_ui_front();
}

void GameView::switch_players(int new_id)
{
    model.switch_players(new_id);

    // for (int i = 0; i < (int)players.size(); i++)
    // {
    //     if (i == new_id)
    //     {
    //         players[i].player->activate();
    //         model.get_tokel(i).set_tokel(true);
    //     }
    //     else
    //     {
    //         model.get_tokel(i).set_tokel(false);
    //         players[i].player->disable();
    //     }
    // }

    // if (!model.get_tokel(new_id).is_active())
    // {
    //     model.set_active_player(-1);
    //     return;
    // }
    // if (model.get_active_player() != -1 && model.get_active_player() != new_id)
    // {
    //     model.get_tokel(model.get_active_player()).set_tokel(false);
    //     players[model.get_active_player()].player->disable();
    // }

    // model.switch_players(new_id);

    // players[new_id].player->activate();
}

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