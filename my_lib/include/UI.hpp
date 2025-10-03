/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   UI.hpp                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kvanden- <kvanden-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/21 18:02:04 by kvanden-          #+#    #+#             */
/*   Updated: 2025/05/13 16:36:21 by kvanden-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "raylib.h"
#include <functional>
#include <iostream>
#include <array>
#include <optional>

enum class UiColors {
    BG,
    FIRST,
    SECOND,
    THIRD,
    HOVER,
    ON,
    OFF,
    BORDER,
    Count
};

class BufferedWin;
class UI
{
private:

    static std::array<Color, static_cast<size_t>(UiColors::Count)> default_colors;
    std::array<std::optional<Color>, static_cast<size_t>(UiColors::Count)> custom_colors{};

    
    protected:
    bool _is_locked = false;
    bool _is_active = true;
    bool _is_visible = true;
    bool is_tabt = false;
    std::function<void(UI &)> callback;
    
    BufferedWin *parent = nullptr;
    
    public:
    UI(std::function<void(UI &)> callback = nullptr) : callback(callback) {}
    virtual ~UI() = default;

    virtual void draw(void) const = 0;
    virtual bool update(void) = 0;
    
    void set_tab(bool value);
    
    virtual bool capture_tab(int direction = 1);
    virtual void remove_tab(void);
    
    void set_callback(std::function<void(UI &)> new_callback);
    void run_callback(void);
    
    void set_active(bool value);
    void set_visible(bool value);
    void disable(void);
    void activate(void);

    void set_lock(bool value);
    
    void set_parent(BufferedWin *parent);
    BufferedWin *get_parent(void) const;
    
    Vector2 get_mouse_pos(void) const;

    Color get_color(UiColors color) const;
    static Color get_dcolor(UiColors color);
    void set_color(UiColors color, Color value);
};
