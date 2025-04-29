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

#include "Win.hpp"

void Win::add_ui(std::unique_ptr<UI> element)
{
    ui_elements.push_back(std::move(element));
}

void Win::rm_tab(void)
{
    ui_elements[current_tab]->remove_tab();
    // current_tab = -1;
}
bool Win::next_tab(bool round)
{
    size_t size = ui_elements.size();
    if (size == 0)
        return (false);

    if (current_tab == -1)
        current_tab = 0;

    int start = current_tab;
    int i = start;

    if (!round && (current_tab == size))
        return (false);
    do
    {
        if (ui_elements[i]->capture_tab())
        {
            current_tab = i;
            return (true);
        }
        i++;
        if (round)
            i %= size;
    } while (round ? i != start : (i != size));
    return (false);
}

bool Win::capture_tab(void)
{
    if (!_is_active)
        return (false);

    if (!next_tab(false))
        current_tab = -1;
    return (current_tab != -1);
}

void Win::update()
{
    if (!_is_active)
        return;
    if (current_tab >= 0 && (GetMouseDelta().x > 0 || GetMouseDelta().y > 0 || IsKeyPressed(KEY_ESCAPE)))
        rm_tab();
    for (size_t i = 0; i < ui_elements.size(); ++i)
    {
        ui_elements[i]->update();
    }
}

void Win::update_tabs(void)
{
    if (!_is_active)
        return;
    if (IsKeyPressed(KEY_TAB))
        next_tab(true);
}

void Win::draw() const
{
    if (!_is_active)
        return;
    for (const auto &element : std::ranges::views::reverse(ui_elements))
    {
        element->draw();
    }
}

UI *Win::get_ui_at(int i) const
{
    return (ui_elements[i].get());
}

int Win::get_num_of_elements() const
{
    return (ui_elements.size());
}