/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   UI.cpp                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kvanden- <kvanden-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/24 12:00:53 by kvanden-          #+#    #+#             */
/*   Updated: 2025/05/13 16:35:41 by kvanden-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "UI.hpp"
#include "BufferedWin.hpp"

std::array<Color, static_cast<size_t>(UiColors::Count)> UI::default_colors = {
    Color{50, 50, 50, 255},     // bg
    Color{70, 70, 70, 255},     // first
    Color{90, 90, 90, 255},     // second
    Color{110, 110, 110, 255},  // third
    Color{255, 255, 255, 30},   // hover
    Color{40, 180, 99, 255},    // ON  
    Color{52, 73, 94, 255},      // OFF
    Color{236, 240, 241, 255},  // BORDER
};

Color UI::get_color(UiColors color) const
{
    auto idx = static_cast<size_t>(color);

    if (custom_colors[idx].has_value())
        return custom_colors[idx].value();
    return default_colors[idx];
}

Color UI::get_dcolor(UiColors color)
{
    return default_colors[static_cast<size_t>(color)];
}

void UI::set_color(UiColors color, Color value)
{
    custom_colors[static_cast<size_t>(color)] = value;
}

bool UI::capture_tab(void)
{
    if (!_is_active)
        return (false);
    is_tabt = !is_tabt;
    return (is_tabt);
}

void UI::set_tab(bool value)
{
    is_tabt = value;
}

void UI::remove_tab(void)
{
    is_tabt = false;
}

void UI::set_callback(std::function<void(UI &)> new_callback)
{
    callback = new_callback;
}

void UI::run_callback()
{
    if (callback)
        callback(*this);
}

void UI::set_active(bool value)
{
    _is_active = value;
}

void UI::set_visible(bool value)
{
    _is_visible = value;
}

void UI::activate(void)
{
    set_active(true);
    set_visible(true);
}

void UI::disable(void)
{
    set_active(false);
    set_visible(false);
}

void UI::set_parent(BufferedWin *parent)
{
    this->parent = parent;
}

BufferedWin *UI::get_parent(void) const
{
    return (this->parent);
}

Vector2 UI::get_mouse_pos(void) const
{
    if (parent)
        return (parent->get_mouse_pos());
    return (GetMousePosition());
}