/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Tokel.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kvanden- <kvanden-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/24 12:18:52 by kvanden-          #+#    #+#             */
/*   Updated: 2025/03/24 16:56:29 by kvanden-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "UIModel.hpp"
#include <string>

class TokelModel : public UIModel
{
private:
    bool active = false;
    bool hover = false;

    std::string text;
    
    public:
    TokelModel(const std::string &text, std::function<void()> callback = nullptr);
    ~TokelModel(void) = default;

    const std::string &get_text(void) const { return text; }
    
    void set_tokel(bool value);
    

    bool is_active(void) const { return active; }
    bool is_hover(void) const { return hover; }
    void set_hover(bool value) { hover = value; }
    void tokel(void) { active = !active; }
};