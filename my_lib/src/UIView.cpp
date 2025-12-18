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

#include "UIView.hpp"
#include "BufferedWin.hpp"

UIView::UIView(UIModel *model): model(model) {}


// std::array<Color, static_cast<size_t>(UiColors::Count)> UIView::default_colors = {
//     Color{40, 50, 55, 255},     // bg
//     Color{53, 60, 67, 255},     // first
//     Color{90, 90, 90, 255},     // second
//     Color{110, 110, 110, 255},  // third
//     Color{255, 255, 255, 30},   // hover
//     Color{40, 180, 99, 255},    // ON  
//     Color{52, 73, 94, 255},      // OFF
//     Color{255, 255, 255, 255},  // TEXT
//     Color{255, 255, 255, 255},   // PLACEHOLDER
//     Color{0, 0, 0, 255},  // BORDER
// };

std::array<Color, static_cast<size_t>(UiColors::Count)> UIView::default_colors = {
    Color{18, 30, 36, 255},     // bg (diep water)
    Color{28, 44, 52, 255},     // first (panel)
    Color{42, 60, 70, 255},     // second (sub-panel)
    Color{62, 82, 92, 255},     // third (raised)
    Color{255, 255, 255, 24},  // hover (subtiel)
    Color{46, 204, 170, 255},  // ON (aqua / water)
    Color{80, 94, 102, 255},   // OFF (neutral)
    Color{235, 245, 250, 255}, // TEXT
    Color{160, 200, 210, 255}, // PLACEHOLDER
    Color{12, 18, 22, 255},    // BORDER
};


Color UIView::get_color(UiColors color) const
{
    auto idx = static_cast<size_t>(color);

    if (custom_colors[idx].has_value())
        return custom_colors[idx].value();
    return default_colors[idx];
}

void UIView::clear_color(UiColors color)
{
    custom_colors[static_cast<size_t>(color)] = std::nullopt;
}

Color UIView::get_dcolor(UiColors color)
{
    return default_colors[static_cast<size_t>(color)];
}

void UIView::set_color(UiColors color, Color value)
{
    custom_colors[static_cast<size_t>(color)] = value;
}

// void UIView::set_lock(bool value)
// {
//     _is_locked = value;
//     if (value)
//         remove_tab();  TODO
// }

bool UIView::capture_tab(int direction)
{
    (void)direction;
    if (model->is_locked())
        return (false);
    is_tabt = !is_tabt;
    return (is_tabt);
}

void UIView::set_tab(bool value)
{
    is_tabt = value;
}

void UIView::remove_tab(void)
{
    is_tabt = false;
}

void UIView::set_parent(BufferedWin *parent)
{
    this->parent = parent;
}

BufferedWin *UIView::get_parent(void) const
{
    return (this->parent);
}

Vector2 UIView::get_mouse_pos(void) const
{
    if (parent)
        return (parent->get_mouse_pos());
    return (GetMousePosition());
}