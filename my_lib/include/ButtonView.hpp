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

#include "UI.hpp"
#include "ButtonModel.hpp"

class ButtonView : public UI
{
private:
    ButtonModel *model;
    const Rectangle bounds;

public:
    ButtonView(ButtonModel *model, float x, float y, float w, float h);
    ~ButtonView() = default;
    void draw(void) const override;
    bool update(void) override;
};