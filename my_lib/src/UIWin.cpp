/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   UIWin.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kvanden- <kvanden-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/24 12:57:06 by kvanden-          #+#    #+#             */
/*   Updated: 2025/03/25 18:21:32 by kvanden-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "UIWin.hpp"

UIWin::UIWin(std::string title, int w, int h, int fps)
    : Win(std::move(title), w, h, fps) {}

void UIWin::add_ui(std::unique_ptr<UI> element)
{
    ui_elements.push_back(std::move(element));
}

void UIWin::rm_tab(void)
{
    std::cout << "removed tab????????  " << current_tab << std::endl;
    current_tab = -1;
    for (auto &element : ui_elements)
    {
        element->remove_tab();
    }
}
void UIWin::next_tab(void)
{
    if (ui_elements.size() == 0)
        return ;
    int start_tab = current_tab;
    int new_tab = current_tab;

    if (current_tab == -1)
    {
        start_tab = 0;
        current_tab = ui_elements.size() - 1;
    }
    do
    {
        new_tab = (new_tab + 1) % ui_elements.size();
        std::cout << "new tab: " << new_tab << std::endl;
        if (ui_elements[new_tab]->set_tab())
        {
            if (new_tab != start_tab)
            ui_elements[current_tab]->remove_tab();
            current_tab = new_tab;
            return;
        }
    } while (new_tab != start_tab);
}

void UIWin::update()
{
    if (IsKeyPressed(KEY_TAB))
        next_tab();
    if (current_tab >= 0 && (GetMouseDelta().x > 0 || GetMouseDelta().y > 0 || IsKeyPressed(KEY_ESCAPE)))
        rm_tab();
    for (auto &element : ui_elements)
    {
        element->update();
    }
}

void UIWin::draw() const
{
    for (const auto &element : ui_elements)
    {
        element->draw();
    }
}
