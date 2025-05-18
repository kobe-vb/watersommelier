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

    this->win = LoadRenderTexture(600, 600);
    this->shader = LoadShader(0, "data/water.fs");

    int radiusLoc = GetShaderLocation(this->shader, "radius");
    int resolutionLoc = GetShaderLocation(shader, "resolution");
    timeLoc = GetShaderLocation(this->shader, "time");

    float radius = 2.0f;
    time = GetTime();
    Vector2 resolution = { (float)win.texture.width, (float)win.texture.height };
    SetShaderValue(shader, radiusLoc, &radius, SHADER_UNIFORM_FLOAT);
    SetShaderValue(shader, timeLoc, &time, SHADER_UNIFORM_FLOAT);
    SetShaderValue(shader, resolutionLoc, &resolution, SHADER_UNIFORM_VEC2);

    this->addParticles(100, RED);
    // this->addParticles(100, GREEN);
}

Sim::~Sim(void)
{
    UnloadRenderTexture(this->win);
    UnloadShader(this->shader);
}

void Sim::addParticles(float n, Color col)
{
    Vector2 pos;
    Vector2 vel;
    for (int i = 0; i < (int)n; i++)
    {
        // pos.x = GetRandomValue(50, 500);
        // pos.y = GetRandomValue(20, 400);
        pos.x = 15 * (i / 10);
        pos.y = 15 * (i % 10);
        
        vel.x = 0;
        vel.y = 0;
        particles.emplace_back(pos, vel, col);
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

    time = GetTime();
    SetShaderValue(shader, timeLoc, &time, SHADER_UNIFORM_FLOAT);


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
    BeginTextureMode(this->win);
    ClearBackground(BLANK);
    for (const auto &p : particles)
    {
        p.draw();
    }
    EndTextureMode();

    BeginShaderMode(shader);
    DrawTextureRec(
        this->win.texture,
        (Rectangle){0, 0, (float)this->win.texture.width, -(float)this->win.texture.height},
        (Vector2){0, 0},
        WHITE
    );
    EndShaderMode();

}