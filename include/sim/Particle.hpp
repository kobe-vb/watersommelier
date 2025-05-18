/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Particle.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kvanden- <kvanden-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/15 17:23:04 by kvanden-          #+#    #+#             */
/*   Updated: 2025/05/15 18:16:46 by kvanden-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#include "raylib.h"

class Particle
{
private:
    Vector2 vel;
    float radius = 5.0f;
    Color col = BLUE;

public:
    Vector2 pos;
    
    Particle(Vector2 pos, Vector2 vel, Color col);
    ~Particle() = default;

    void update(float dt);
    void draw(void) const;
};
