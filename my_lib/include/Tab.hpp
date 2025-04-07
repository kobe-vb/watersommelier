/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Tab.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kvanden- <kvanden-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/24 12:03:41 by kvanden-          #+#    #+#             */
/*   Updated: 2025/03/25 18:42:36 by kvanden-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "Tokel.hpp"
#include <vector>
#include <string>

class Tab : public UI
{
private:
    std::vector<Tokel> tabs;
    size_t current_tab = 0;
public:
    Tab(int x, int y, int w, int h, std::initializer_list<std::string> tabs);
    void draw(void) const override;
    void update(void) override;
    bool set_tab(void) override;
    void remove_tab(void) override;
};
