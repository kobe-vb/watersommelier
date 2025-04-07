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

#include "Win.hpp"

Win::Win(std::string title, int w, int h, int fps)
: is_running(true)
{
    InitWindow(w, h, title.c_str());
    SetTargetFPS(fps);
    SetExitKey(KEY_Q);
}

Win::~Win()
{
    CloseWindow();
}

void Win::run()
{
    while (is_running && !WindowShouldClose())
    {
        update();
        BeginDrawing();
        draw();
        EndDrawing();
    }
}