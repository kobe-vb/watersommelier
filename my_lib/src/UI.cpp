/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   UI.cpp                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kvanden- <kvanden-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/24 12:00:53 by kvanden-          #+#    #+#             */
/*   Updated: 2025/03/24 18:38:36 by kvanden-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "UI.hpp"

const bool UI::get_tab(void) const
{
    return this->tab;
}

bool UI::set_tab(void)
{
    tab = true;
    return (true);
}

void UI::remove_tab(void) {tab = false;}

void UI::set_callback(std::function<void(UI&)> new_callback)
{
    callback = new_callback;
}

void UI::run_callback()
{
    if (callback)
        callback(*this);
}