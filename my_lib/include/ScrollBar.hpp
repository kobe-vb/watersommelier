/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ScrollBar.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kvanden- <kvanden-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/13 17:22:25 by kvanden-          #+#    #+#             */
/*   Updated: 2025/05/13 18:27:11 by kvanden-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "raylib.h"

#include <functional>

class ScrollBar
{
    private:
        Rectangle bounds;
        Rectangle scroll_bar;
        float winheight;
        float scrollheight;
        float scrollpos;
        
        std::function<void(float)> callback;
        
        void scroll_update(void);
        bool IsMouseOver() const;
        
    public:
        ScrollBar(Rectangle bouns, float winheight, float scrollheight, std::function<void(float )> callback = nullptr);
        ~ScrollBar() = default;
        
        void add_height(float height);
        float get_height();
        void scroll(float height);

        void draw(void) const;
        void update(void);
};