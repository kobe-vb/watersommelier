/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Text.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kvanden- <kvanden-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/24 15:16:47 by kvanden-          #+#    #+#             */
/*   Updated: 2025/03/24 18:52:22 by kvanden-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "UI.hpp"
#include <string>

class Text : public UI
{
private:
    bool is_hover;
    bool is_active = false;
    Rectangle bounds;
    std::string text;
    const std::string tmp;

public:
    Text(float x, float y, float w, float h, std::function<void(UI&)> callback = nullptr, const std::string &tmp = "");
    void draw(void) const override;
    void update(void) override;
    std::string &get_text(void);
    const Rectangle &get_rect(void); 
    bool capture_tab(void) override;
    void move(int x, int y);
    void reset(void);
};