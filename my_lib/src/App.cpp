/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   App.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kvanden- <kvanden-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/13 18:53:08 by kvanden-          #+#    #+#             */
/*   Updated: 2025/05/14 17:18:05 by kvanden-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "App.hpp"

App::App(std::string title, int w, int h, int fps)
    : is_running(true)
{
    SetTraceLogLevel(LOG_ALL);
    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    InitWindow(w, h, title.c_str());
    SetTargetFPS(fps);
    SetExitKey(0);
    MaximizeWindow();

    Image icon = LoadImage("data/img/mijn_icoon.png");
    SetWindowIcon(icon);
    UnloadImage(icon);
}

App::~App()
{
    CloseWindow();
}

void App::run()
{
    while (is_running && !WindowShouldClose())
    {
        if (IsKeyPressed(KEY_F11))
            ToggleFullscreen();
        update();
        BeginDrawing();
        draw();
        EndDrawing();
    }
}