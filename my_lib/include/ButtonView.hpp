/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ButtonView.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kvanden- <kvanden-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/24 14:46:15 by kvanden-          #+#    #+#             */
/*   Updated: 2025/03/24 17:11:25 by kvanden-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "UIView.hpp"
#include "ButtonModel.hpp"

class ButtonView : public UIView
{
private:
    ButtonModel *model;
    Rectangle bounds;

public:
    ButtonView(ButtonModel *model, float x, float y, float w, float h, bool center = false);
    ~ButtonView() = default;
    void draw(void) const override;
    bool update(void) override;
};