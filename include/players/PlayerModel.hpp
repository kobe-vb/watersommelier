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
#include "GlassModel.hpp"
#include "GameData.hpp"
#include "ScrollBar.hpp"
#include "History.hpp"
#include "Sim.hpp"
#include "TokelModel.hpp"
#include "WebsiteModel.hpp"
#include "CSVDownloader.hpp"

enum class Role
{
    Player,
    Sudo
};

class GameModel;

// TODO: refactor player heeft versilende fases, data verzamel score, lenen, qr/bednqking
class PlayerModel : public UIModel
{
protected:
    const std::string name;
    GlassModel glass;
    ScoreGlassModel scoreGlass;
    WebsiteModel website;

    History history;

    GameModel *game;

private:
    std::string code;

public:
    PlayerModel(const std::string &name, GameData &data, Sim &sim, GameModel *game);
    ~PlayerModel() = default;

    Role role = Role::Player;

    PlayerModel *thief = nullptr;

    void update(void);

    void next_glass();
    bool steal_glass();
    void score_glass();

    void save_stolen_glass(GlassModel &glass, ScoreGlassModel &scoreGlass);

    void fiks_sim(void);

    bool set_code(const std::string &code);
    bool take_code_for_dropdown(const std::string &code);
    bool is_my_code(const std::string &code) const;
    bool is_me(const std::string &key) const { return key == name || key == code; }

    void set_data(std::string website_data);
    void set_website(std::string website);

    void save_data(CSVDownloader &csv, GameData &data);
    std::string to_json(void) const;

    void demo(void);

    const std::string &get_code(void) const { return (code); }
    const std::string &get_name(void) const { return (name); }

    GlassModel &get_glass(void) { return (glass); }
    ScoreGlassModel &get_score_glass(void) { return (scoreGlass); }
    WebsiteModel &get_websiteModel(void) { return (website); }

    void draw_history(void) { history.draw(); }
    void remove_code(void) { code = ""; }
};