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

# pragma once

#include "BufferedWin.hpp"
#include "GlassModel.hpp"
#include "GameData.hpp"
#include "ScrollBar.hpp"
#include "History.hpp"
#include "Sim.hpp"
# include "TokelModel.hpp"
#include "qrcodegen.hpp"
#include "WebsiteData.hpp"

#include <optional>


// TODO: refactor player heeft versilende fases, data verzamel score, lenen, qr/bednqking
class PlayerModel: public UIModel
{
protected:
    const std::string name;
    GlassModel glass;
    ScoreGlassModel scoreGlass;
    private:
    
    std::string code;
    
    // TODO: refactor
    std::optional<qrcodegen::QrCode> qr;
    std::string website;
    WebsiteData website_data;
    
    public:
    PlayerModel(const std::string &name, GameData &data, Sim &sim);
    ~PlayerModel() = default;
    
    History history;
    
    void update(void);

    void next_glass();
    bool steal_glass();
    void score_glass();

    void fiks_sim(void);

    bool set_code(const std::string &code);
    bool take_code_for_dropdown(const std::string &code);
    bool is_my_code(const std::string &code) const;

    void set_website(std::string website);
    void set_website_data(std::string website_data);
    
    void save_data(std::ofstream &file, size_t &counter, GameData &data);
    std::string to_json(void) const;

    void demo(void);

    const std::string &get_code(void) const { return (code); }
    const std::string &get_name(void) const { return (name); }

    GlassModel &get_glass(void) { return (glass); }
    ScoreGlassModel &get_score_glass(void) { return (scoreGlass); }
};