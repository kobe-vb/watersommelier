/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Win.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kvanden- <kvanden-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/13 18:51:55 by kvanden-          #+#    #+#             */
/*   Updated: 2025/03/24 18:44:10 by kvanden-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "raylib.h"
#include <string>
#include <iostream>

class Win
{
    private:
        bool is_running;

    public:
        Win(std::string title, int w, int h, int fps);
        ~Win();

        void run();
        virtual void update() = 0;
        virtual void draw() const = 0;
};