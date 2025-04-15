/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   UI.hpp                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kvanden- <kvanden-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/21 18:02:04 by kvanden-          #+#    #+#             */
/*   Updated: 2025/03/24 18:52:01 by kvanden-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "raylib.h"

#include <functional>

#include <iostream>

class UI
{
private:
protected:
    bool _is_active = true;
    bool is_tabt = false;
    std::function<void(UI &)> callback;

public:
    UI(std::function<void(UI &)> callback = nullptr) : callback(callback) {}
    virtual ~UI() = default;

    virtual void draw(void) const = 0;
    virtual void update(void) = 0;

    virtual bool capture_tab(void);
    virtual void remove_tab(void);

    void set_callback(std::function<void(UI &)> new_callback);
    void run_callback(void);
    void set_active(bool val);
};