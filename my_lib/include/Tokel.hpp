/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Tokel.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kvanden- <kvanden-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/24 12:18:52 by kvanden-          #+#    #+#             */
/*   Updated: 2025/03/24 16:56:29 by kvanden-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "UI.hpp"
#include <string>

class Tokel : public UI
{
private:
    bool active;
    bool is_hover;
    const Rectangle bounds;
    const std::string text;

public:
    Tokel(float x, float y, float w, float h, const std::string &text, std::function<void(UI&)> callback = nullptr);
    ~Tokel(void);
    void draw(void) const override;
    void update(void) override;
};