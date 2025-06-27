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
#include "Glass.hpp"
#include "GameData.hpp"
#include "ScrollBar.hpp"
#include "History.hpp"
#include "Sim.hpp"
# include "Tokel.hpp"
#include "qrcodegen.hpp"

#include <optional>

class Player: public Win
{
private:
    const std::string name;

    Glass glass;
    History history;
    Rectangle rect;

    std::string code;

    std::optional<qrcodegen::QrCode> qr;
    std::string website;

    void draw_qr(void) const;
    
public:
    Player(std::string &name, GameData &data, Sim &sim);
    ~Player() = default;

    bool capture_tab(void) override;

    void next_glass(UI &ui);

    void fiks_sim(void);

    bool take_code(std::string &code);
    bool is_my_code(std::string &code) const;

    void update(void) override;
    void draw(void) const override;

    void set_website(std::string website);

    const std::string &get_name(void) const { return (name); }
    
    void save_data(std::ofstream &file, size_t &counter);
};


struct Player_data
{
    Tokel *tokel;
    Player *player;
};