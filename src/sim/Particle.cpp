/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Particle.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kvanden- <kvanden-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/15 17:23:39 by kvanden-          #+#    #+#             */
/*   Updated: 2025/05/15 18:18:15 by kvanden-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Particle.hpp"


Particle::Particle(Vector2 pos, Vector2 vel) : vel(vel), pos(pos)
{}

void Particle::update(float dt)
{
    this->pos.x += (this->vel.x * dt);
    this->pos.y += (this->vel.y * dt);
}

void Particle::draw(void) const
{
    DrawCircleV(this->pos, this->radius, this->color);
}
