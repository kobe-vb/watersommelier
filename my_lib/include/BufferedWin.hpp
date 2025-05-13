/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   BufferedWin.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kvanden- <kvanden-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/13 15:52:27 by kvanden-          #+#    #+#             */
/*   Updated: 2025/05/13 16:41:31 by kvanden-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "Win.hpp"

class BufferedWin : public Win
{
    protected:
        RenderTexture2D win;
        Vector2 pos;

    public:
        BufferedWin(float x, float y, float w, float h);
        ~BufferedWin();

        void add_ui(std::unique_ptr<UI> element) override;
        void draw() const override;

        Vector2 get_mouse_pos(void) const;
        void move(int x, int y);
        void set_pos(int x, int y);
        virtual void clear(void) const;
};