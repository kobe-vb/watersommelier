/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Particle.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kvanden- <kvanden-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/15 17:23:39 by kvanden-          #+#    #+#             */
/*   Updated: 2025/05/20 22:08:19 by kvanden-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Particle.hpp"
#include <cmath>


Particle::Particle(Vector2 pos, Vector2 vel, Color col) :
vel(vel), col(col), pos(pos)
{}

void Particle::resolveBounds(const Rectangle& bounds)
{
    if (pos.x - radius < bounds.x) {
        pos.x = bounds.x + radius;
        vel.x *= -0.7f; // Iets meer energieverlies bij botsing
    } else if (pos.x + radius > bounds.x + bounds.width) {
        pos.x = bounds.x + bounds.width - radius;
        vel.x *= -0.7f;
    }

    if (pos.y - radius < bounds.y) {
        pos.y = bounds.y + radius;
        vel.y *= -0.7f;
    } else if (pos.y + radius > bounds.y + bounds.height) {
        pos.y = bounds.y + bounds.height - radius;
        vel.y *= -0.7f;
    }
    
    // Demping toevoegen om tot rust te komen
    vel.x *= 0.995f;
    vel.y *= 0.995f;
}

void Particle::interact(Particle& other)
{
    float dx = other.pos.x - pos.x;
    float dy = other.pos.y - pos.y;
    float dist2 = dx * dx + dy * dy;
    float minDist = (radius + other.radius) * 2.0f; // Iets meer ruimte tussen deeltjes

    if (dist2 < minDist * minDist && dist2 > 0.01f)
    {
        // Afstoting bij botsing
        float dist = sqrtf(dist2);
        float overlap = minDist - dist;
        
        // Normaal vector
        float nx = dx / dist;
        float ny = dy / dist;
        
        // Corrigeer posities om overlap te voorkomen
        float correction = overlap * 0.5f;
        pos.x -= nx * correction;
        pos.y -= ny * correction;
        other.pos.x += nx * correction;
        other.pos.y += ny * correction;
        
        // Botsingskracht toepassen
        float strength = 0.3f * overlap; // Zachte afstoting voor vloeistofgedrag
        Vector2 force = { nx * strength, ny * strength };
        
        applyForce({ -force.x, -force.y });
        other.applyForce(force);
        
        // Energieverlies bij botsing
        vel.x *= 0.98f;
        vel.y *= 0.98f;
        other.vel.x *= 0.98f;
        other.vel.y *= 0.98f;
    }
    else if (col.r == other.col.r && col.g == other.col.g && col.b == other.col.b)
    {
        // Cohesie tussen deeltjes van dezelfde kleur
        float dist = sqrtf(dist2);
        if (dist < 200.0f && dist > minDist) // Kleinere invloedsafstand
        {
            float strength = 5.0f * (1.0f - dist / 50.0f); // Zwakker naarmate afstand groter wordt
            Vector2 attract = { (dx / dist) * strength, (dy / dist) * strength };
            applyForce(attract);
            other.applyForce({ -attract.x, -attract.y });
        }
    }
}


void Particle::applyForce(Vector2 force)
{
    vel.x += force.x;
    vel.y += force.y;
    
    // Beperk maximale snelheid voor stabiliteit
    float maxSpeed = 100.0f;
    float speed = sqrtf(vel.x * vel.x + vel.y * vel.y);
    if (speed > maxSpeed) {
        vel.x = (vel.x / speed) * maxSpeed;
        vel.y = (vel.y / speed) * maxSpeed;
    }
}

void Particle::update(float dt)
{
    // Verminderde zwaartekracht voor rustigere beweging
    vel.y += 520.0f * dt;
    
    // Viscositeit simuleren (weerstand in vloeistof)
    vel.x *= 0.999f;
    vel.y *= 0.999f;

    pos.x += vel.x * dt;
    pos.y += vel.y * dt;
}

void Particle::draw(void) const
{
    DrawCircleV(this->pos, this->radius, this->col);
}