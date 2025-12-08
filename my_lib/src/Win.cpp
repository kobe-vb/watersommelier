/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Win.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kvanden- <kvanden-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/24 12:57:06 by kvanden-          #+#    #+#             */
/*   Updated: 2025/05/19 12:27:28 by kvanden-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Win.hpp"

void Win::add_ui(std::unique_ptr<UIView> element)
{
    ui_elements.push_back(std::move(element));
}

void Win::remove_tab(void)
{
    if (current_tab == -1 || current_tab >= (int)ui_elements.size())
        return;
    ui_elements[current_tab]->remove_tab();
    // current_tab = -1;
}
bool Win::next_tab(int direction, bool round)
{
    int size = (int)ui_elements.size();
    if (size == 0)
        return (false);

    if (current_tab == -1)
        current_tab = direction > 0 ? 0 : size - 1;

    int start = current_tab;
    int i = start;

    if (!round && (current_tab == size))
        return (false);
    do
    {
        if (ui_elements[i]->capture_tab(direction))
        {
            current_tab = i;
            return (true);
        }
        i += direction;
        if (round)
            i = (i % size + size) % size;
        if (i < 0)
            return (false);
    } while (round ? i != start : (i != size));
    return (false);
}

bool Win::capture_tab(int direction)
{
    if (model->is_locked())
        return (false);

    if (!next_tab(direction, false))
        current_tab = -1;
    return (current_tab != -1);
}

bool Win::update()
{
    if (model->is_locked())
        return false;
    int temp = current_tab;
    if (current_tab >= 0 && (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) || IsKeyPressed(KEY_ESCAPE)))
        remove_tab();
    size_t size = ui_elements.size();
    for (size_t i = 0; i < size; ++i)
    {
        if (ui_elements[i]->update())
            current_tab = i;
        if (ui_elements_is_changed)
        {
            ui_elements_is_changed = false;
            break;
        }
    }
    return (temp != current_tab);
}

void Win::set_current_tab(int i)
{
    if (i >= (int)ui_elements.size())
        throw std::out_of_range("Index out of range");
    current_tab = i;
    ui_elements[i]->set_tab(true);
}

void Win::update_tabs(void)
{
    if (model->is_locked())
        return;
    if (IsKeyDown(KEY_LEFT_SHIFT) && IsKeyPressed(KEY_TAB))
        next_tab(-1, true);
    else if (IsKeyPressed(KEY_TAB))
        next_tab(1, true);
}

void Win::draw() const
{
    if (!model->is_visible())
        return;
    // for (const auto &element : std::ranges::views::reverse(ui_elements))
    // {

    for (auto it = ui_elements.rbegin(); it != ui_elements.rend(); ++it)
    {
        const auto &element = *it;
        element->draw();
    }
}

UIView *Win::get_ui_at(int i) const
{
    return (ui_elements[i].get());
}

UIView *Win::get_last_ui() const
{
    return (ui_elements.back().get());
}

int Win::get_num_of_elements() const
{
    return (ui_elements.size());
}

void Win::pop_ui_back()
{
    this->remove_tab();
    if (!ui_elements.empty())
        ui_elements.pop_back();
    current_tab = std::min(current_tab, (int)ui_elements.size() - 1);
    ui_elements_is_changed = true;
}

void Win::pop_ui_front()
{
    this->remove_tab();
    if (!ui_elements.empty())
        ui_elements.erase(ui_elements.begin());
    ui_elements_is_changed = true;
}

void Win::clear_ui(void)
{
    this->remove_tab();
    ui_elements.clear();
    ui_elements_is_changed = true;
}
