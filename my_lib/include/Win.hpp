/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Win.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kvanden- <kvanden-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/24 12:53:25 by kvanden-          #+#    #+#             */
/*   Updated: 2025/05/19 12:27:09 by kvanden-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "UI.hpp"

#include <vector>
#include <memory>
#include <iostream>
#include <ranges>

class Win : public UI
{
private:
    std::vector<std::unique_ptr<UI>> ui_elements;
    bool next_tab(bool round);
    void remove_tab(void) override;
    int current_tab = -1;

public:
    Win() = default;
    ~Win() = default;
    virtual void add_ui(std::unique_ptr<UI> element);
    void update() override;
    void draw() const override;

    void update_tabs(void);
    bool capture_tab(void) override;
    UI *get_ui_at(int i) const;
    int get_num_of_elements() const;
    void pop_ui(void);
    void set_current_tab(int i);
};
