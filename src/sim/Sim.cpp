/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Sim.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kvanden- <kvanden-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/15 17:20:57 by kvanden-          #+#    #+#             */
/*   Updated: 2025/05/20 21:59:17 by kvanden-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Sim.hpp"
#include "Settings.hpp"


#include <iostream>
#include <UI.hpp>

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

    glass.x = 0;
    glass.y = 0;
    glass.width = 600;
    glass.height = 600;
}

Sim::~Sim(void)
{
    UnloadRenderTexture(this->win);
    UnloadShader(this->shader);
}

void Sim::reset(void)
{
    this->particles.clear();
    this->spatialMap.clear();
}

void Sim::set_rect(void)
{    
    rect.height = GetRenderHeight() - (PEDING * 2);
    rect.width = (GetScreenWidth() * 1 / 3) - (PEDING * 3);
    rect.x = PEDING + (GetScreenWidth() * 2 / 3);
    rect.y = PEDING;

}

void Sim::addParticles(float n, Color col)
{
    // Betere initiële positie voor vloeistofgedrag
    // Deeltjes worden onderaan het glas geplaatst met lage beginsnelheid
    for (int i = 0; i < (int)n; i++)
    {
        // Verdeel de deeltjes over de breedte van het glas, onderaan
        float x = glass.width * ((float)rand() / RAND_MAX) * 0.8f + glass.width * 0.1f;
        float y = glass.height * 0.7f + (glass.height * 0.2f * ((float)rand() / RAND_MAX));
        
        Vector2 pos = {x, y};
        
        // Lage, willekeurige beginsnelheid
        Vector2 vel = {
            ((float)rand() / RAND_MAX * 2.0f - 1.0f) * 5.0f,  // -5 tot 5
            ((float)rand() / RAND_MAX * 2.0f - 1.0f) * 5.0f   // -5 tot 5
        };
        
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

    // Zoek in een groter gebied voor betere vloeistofsimulatie
    for (int dx = -2; dx <= 2; dx++)
    {
        for (int dy = -2; dy <= 2; dy++)
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
    // Beperk tijdstap voor stabiliteit
    float maxDt = 0.016f;  // Max 16ms tijdstap
    if (dt > maxDt) dt = maxDt;

    time = GetTime();
    SetShaderValue(shader, timeLoc, &time, SHADER_UNIFORM_FLOAT);

    spatialMap.clear();

    for (auto &p : particles)
    {
        insertIntoSpatialMap(p);
    }

    // Meerdere sub-stappen voor betere stabiliteit
    const int substeps = 3;
    float subDt = dt / substeps;
    
    for (int step = 0; step < substeps; step++)
    {
        for (auto &p : particles)
        {
            auto neighbors = getNeighbors(p);
            for (auto &n : neighbors)
            {
                p.interact(*n);
            }
            p.update(subDt);
            p.resolveBounds(glass);
        }
    }

    // Voeg relaxatie stap toe
    relaxFluid();
}

void Sim::relaxFluid()
{
    // Eenvoudige relaxatie om het vloeistofveld te stabiliseren
    for (auto &p : particles)
    {
        auto neighbors = getNeighbors(p);
        
        // Bereken lokale dichtheid
        int count = 0;
        for (auto &n : neighbors)
        {
            float dx = n->pos.x - p.pos.x;
            float dy = n->pos.y - p.pos.y;
            float dist2 = dx * dx + dy * dy;
            
            if (dist2 < 25.0f * 25.0f)  // 25.0 is ongeveer 5 keer de deeltjesradius
                count++;
        }
        
        // Als er te veel deeltjes zijn, pas een lichte repulsie toe
        if (count > 12)
        {
            for (auto &n : neighbors)
            {
                float dx = n->pos.x - p.pos.x;
                float dy = n->pos.y - p.pos.y;
                float dist2 = dx * dx + dy * dy;
                
                if (dist2 > 0.001f && dist2 < 25.0f * 25.0f)
                {
                    float dist = sqrtf(dist2);
                    float force = 0.01f * (1.0f - dist / (25.0f));
                    
                    Vector2 repel = { (dx / dist) * force, (dy / dist) * force };
                    p.applyForce({ -repel.x, -repel.y });
                }
            }
        }
    }
}

void Sim::draw() const
{
    DrawRectangleRounded(rect, ROUNDED, 10, UI::get_dcolor(UiColors::FIRST));
    DrawRectangleRoundedLinesEx(rect, ROUNDED, 10, 6.0f, BLACK);
    
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
        (Vector2){rect.x + PEDING * 2, rect.y + LINE + PEDING * 2},
        WHITE
    );
    EndShaderMode();
}