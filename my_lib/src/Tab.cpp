/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Tab.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kvanden- <kvanden-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/24 12:11:45 by kvanden-          #+#    #+#             */
/*   Updated: 2025/03/25 18:44:10 by kvanden-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Tab.hpp"

Tab::Tab(int x, int y, int w, int h, std::initializer_list<std::string> tabs)
{
    size_t i = 0;
    for (std::string tab : tabs)
    {
        this->tabs.push_back(Tokel(x + i * w, y, w, h, tab));
        i++;
    }
}

void Tab::draw(void) const
{
    for (Tokel tab : tabs)
        tab.draw();   
}

void Tab::update(void)
{
    for (Tokel &tab : tabs)
        tab.update();
}

bool Tab::set_tab(void)
{
    tab = true;
    return (false);
}

void Tab::remove_tab(void)
{
    tab = false;
}
