/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Sim.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kvanden- <kvanden-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/15 17:20:57 by kvanden-          #+#    #+#             */
/*   Updated: 2025/05/15 18:16:54 by kvanden-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Sim.hpp"

Sim::Sim(void)
{
    this->addParticles(100, RED);
}

void Sim::addParticles(float n, Color col)
{
    Vector2 pos;
    Vector2 vel;
    for (int i = 0; i < (int)n; i++)
    {
        pos.x = GetRandomValue(50, 100);
        pos.y = GetRandomValue(20, 50);
        vel.x = 0;
        vel.y = 0;
        particles.emplace_back(pos, vel);
    }
}

int64_t Sim::hash(int x, int y) const
{
    const int64_t p1 = 73856093;
    const int64_t p2 = 19349663;
    return (int64_t)(x * p1) ^ (y * p2);
}

void Sim::insertIntoSpatialMap(Particle &p)
{
    int cellX = (int)(p.pos.x / cellSize);
    int cellY = (int)(p.pos.y / cellSize);
    spatialMap[hash(cellX, cellY)].push_back(&p);
}

std::vector<Particle *> Sim::getNeighbors(const Particle &p)
{
    std::vector<Particle *> result;
    int cellX = (int)(p.pos.x / cellSize);
    int cellY = (int)(p.pos.y / cellSize);

    for (int dx = -1; dx <= 1; dx++)
    {
        for (int dy = -1; dy <= 1; dy++)
        {
            int64_t h = hash(cellX + dx, cellY + dy);
            auto it = spatialMap.find(h);
            if (it != spatialMap.end())
            {
                for (Particle *other : it->second)
                {
                    if (other != &p)
                        result.push_back(other);
                }
            }
        }
    }

    return result;
}
void Sim::update(float dt)
{
    spatialMap.clear();

    for (auto &p : particles)
    {
        insertIntoSpatialMap(p);
    }

    for (auto &p : particles)
    {
        auto neighbors = getNeighbors(p);
        // TODO: implement a force calculation
        p.update(dt);
    }
}

void Sim::draw() const
{
    for (const auto &p : particles)
    {
        p.draw();
    }
}