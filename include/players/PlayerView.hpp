/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Player.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kvanden- <kvanden-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/24 11:24:37 by kvanden-          #+#    #+#             */
/*   Updated: 2025/05/15 16:23:25 by kvanden-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "BufferedWin.hpp"
#include "GameData.hpp"
#include "ScrollBar.hpp"
#include "Sim.hpp"
#include "qrcodegen.hpp"
// #include "WebsiteData.hpp"

#include "PlayerModel.hpp"
#include "GlassView.hpp"

#include <optional>
#include "ScoreGlassView.hpp"

class PlayerView : public Win
{

private:
    PlayerModel *model;

    GlassView glass;
    ScoreGlassView score_glass;

    void draw_qr(void) const;

protected:
    Rectangle rect;
    Rectangle players_info_rect;

public:
    PlayerView(PlayerModel *model);
    ~PlayerView() = default;

    bool capture_tab(int direction) override;

    bool update(void) override;
    void draw(void) const override;
};
