/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   UIWin.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kvanden- <kvanden-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/24 12:53:25 by kvanden-          #+#    #+#             */
/*   Updated: 2025/03/25 18:08:27 by kvanden-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "Win.hpp"
#include "UI.hpp"

# include <vector>
# include <memory>

class UIWin : public Win
{
    private:
        std::vector<std::unique_ptr<UI>> ui_elements;
        void next_tab(void);
        void rm_tab(void);
        int current_tab = -1;
    public:
        UIWin(std::string title, int w, int h, int fps);
        void add_ui(std::unique_ptr<UI> element);
        void update() override;
        void draw() const override;
};
