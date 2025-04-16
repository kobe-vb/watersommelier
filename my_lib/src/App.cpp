/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Win.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kvanden- <kvanden-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/13 18:53:08 by kvanden-          #+#    #+#             */
/*   Updated: 2025/03/24 14:43:27 by kvanden-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "App.hpp"

App::App(std::string title, int w, int h, int fps)
: is_running(true)
{
    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    InitWindow(w, h, title.c_str());
    SetTargetFPS(fps);
    SetExitKey(NULL);

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
        update();
        BeginDrawing();
        draw();
        EndDrawing();
    }
}