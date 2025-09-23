/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Button.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kvanden- <kvanden-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/24 14:46:15 by kvanden-          #+#    #+#             */
/*   Updated: 2025/03/24 17:11:25 by kvanden-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "UI.hpp"
#include <string>

class Button : public UI
{
private:
    bool is_hover;
    const Rectangle bounds;
    std::string text;

public:
    Button(float x, float y, float w, float h, const std::string &text, std::function<void(UI &)> callback = nullptr);
    ~Button(void);
    void draw(void) const override;
    void update(void) override;
    void set_text(const std::string &text);
};