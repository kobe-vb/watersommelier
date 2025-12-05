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

#include "UIView.hpp"

#include <vector>
#include <memory>
#include <iostream>
#include <ranges>

class Win : public UIView
{
private:
    std::vector<std::unique_ptr<UIView>> ui_elements;
    bool next_tab(int direction, bool round);
    void remove_tab(void) override;
    int current_tab = -1;

    bool ui_elements_is_changed = false;

public:
    Win(UIModel *model) : UIView(model) {};
    ~Win() = default;
    virtual void add_ui(std::unique_ptr<UIView> element);
    bool update() override;
    void draw() const override;

    void update_tabs(void);
    bool capture_tab(int direction) override;
    UIView *get_ui_at(int i) const;
    UIView *get_last_ui() const;
    int get_num_of_elements() const;
    void set_current_tab(int i);
    void pop_ui_back(void);
    void pop_ui_front(void);
};
