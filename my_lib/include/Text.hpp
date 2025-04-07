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
    bool is_active = false;
    bool is_hover;
    const Rectangle bounds;
    std::string text;

public:
    Text(float x, float y, float w, float h, std::function<void(UI&)> callback = nullptr);
    void draw(void) const override;
    void update(void) override;
    bool set_tab(void) override;
    void remove_tab(void) override;
    std::string &get_text(void);
};