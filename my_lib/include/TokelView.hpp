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

#include "UIView.hpp"
#include "TokelModel.hpp"
#include <string>

class TokelView : public UIView
{
private:
    TokelModel *model;
    const Rectangle bounds;

    std::string display_text;
    float textX;
    float textY;

public:
    TokelView(TokelModel *model, float x, float y, float w, float h);
    ~TokelView() = default;

    void draw(void) const override;
    bool update(void) override;

};