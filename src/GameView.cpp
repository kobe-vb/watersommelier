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
#include "MyDraw.hpp"
#include <SudoPlayerModel.hpp>
#include <SudoPlayerView.hpp>

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

    if (isIdle)
        pause_view.draw();
}

void GameView::create_new_player(void)
{
    const std::string &name = model.get_name_input().get_text();

    if (!DEBUG && (model.name_is_taken(name) || name.empty()))
    {
        nameInput->set_color(UiColors::SECOND, RED);
        return;
    }

    nameInput->clear_color(UiColors::SECOND);

    model.reset_sim();

    int ind = model.create_player();

    PlayerModel *pm = model.get_player_by_id(ind);
    std::unique_ptr<PlayerView> pl;
    if (pm->role == Role::Sudo)
        pl = std::make_unique<SudoPlayerView>(model.get_player_by_id(ind));
    else
        pl = std::make_unique<PlayerView>(model.get_player_by_id(ind));

    if (name == "demo")
        model.get_player_by_id(ind)->demo();

    const auto &rect = nameInput->get_rect();
    auto tokel = std::make_unique<TokelView>(&model.get_tokel(ind), rect.x, rect.y, rect.width, rect.height);

    players.push_back({tokel.get(), pl.get()});

    win.add_ui(std::move(tokel));
    win.add_ui(std::move(pl));

    model.get_name_input().reset();
    nameInput->move(PLAYER_WIDTH + PLAYER_PEDING, 0);
    model.switch_players(ind);

    if (players.size() == 9)
    {
        model.get_name_input().set_text("tiboon");
        model.get_name_input().run_callback();
        return;
    }

    if (players.size() == 10 || model.name_is_taken("tiboon"))
        win.pop_ui_front();
}

void GameView::update()
{

    activity.update();
    this->isIdle = activity.seconds_since_activity() > 2;
    if (isIdle)
        pause_view.update();
    
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

    this->win.update();
    this->win.update_tabs();
    model.update();
}