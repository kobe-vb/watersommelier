/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   UI.cpp                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kvanden- <kvanden-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/24 12:00:53 by kvanden-          #+#    #+#             */
/*   Updated: 2025/05/06 18:14:59 by kvanden-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "UI.hpp"

bool UI::capture_tab(void)
{
    if (!_is_active)
        return (false);
    is_tabt = !is_tabt;
    return (is_tabt);
}

void UI::remove_tab(void)
{
    is_tabt = false;
}

void UI::set_callback(std::function<void(UI&)> new_callback)
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