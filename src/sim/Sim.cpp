/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Sim.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kvanden- <kvanden-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/15 17:20:57 by kvanden-          #+#    #+#             */
/*   Updated: 2025/11/14 00:00:00 by kvanden-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Sim.hpp"
#include "Settings.hpp"
#include "Particle.hpp"
#include <UI.hpp>
#include <thread>
#include <vector>
#include <algorithm>

Sim::Sim(void)
{
    this->win = LoadRenderTexture(600, 600);
    this->shader = LoadShader(0, "data/water.fs");

    timeLoc = GetShaderLocation(this->shader, "time");
    int resolutionLoc = GetShaderLocation(shader, "resolution");
    
    time = GetTime();
    Vector2 resolution = { (float)win.texture.width, (float)win.texture.height };
    SetShaderValue(shader, timeLoc, &time, SHADER_UNIFORM_FLOAT);
    SetShaderValue(shader, resolutionLoc, &resolution, SHADER_UNIFORM_VEC2);

    glass.x = 0;
    glass.y = 0;
    glass.width = 600;
    glass.height = 600;
    
    // Optimaliseer cell size voor spatial hash
    cellSize = 50.0f;  // Iets groter voor minder overhead
    
    // Thread count bepalen
    threadCount = std::thread::hardware_concurrency();
    if (threadCount == 0) threadCount = 4;  // Fallback
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
    rect.width = ((GetScreenWidth() - (UI_BORDER * 4) - (PEDING * 2)) * 1 / 3);
    rect.height = GetScreenHeight() - LINE - PEDING - UI_BORDER;

    rect.x = UI_BORDER * 2 + (rect.width + PEDING) * 2;
    rect.y = LINE;
}

void Sim::addParticles(float n, Color col)
{
    // Betere initiële posities voor vloeistofgedrag
    for (int i = 0; i < (int)n; i++)
    {
        float x = glass.width * ((float)rand() / RAND_MAX) * 0.8f + glass.width * 0.1f;
        float y = glass.height * 0.7f + (glass.height * 0.2f * ((float)rand() / RAND_MAX));
        
        // Lage, willekeurige beginsnelheid
        float vx = ((float)rand() / RAND_MAX * 2.0f - 1.0f) * 5.0f;
        float vy = ((float)rand() / RAND_MAX * 2.0f - 1.0f) * 5.0f;
        
        particles.addParticle(x, y, vx, vy, col);
    }
}

int64_t Sim::hash(int x, int y) const
{
    const int64_t p1 = 73856093;
    const int64_t p2 = 19349663;
    return (int64_t)(x * p1) ^ (y * p2);
}

void Sim::buildSpatialMap()
{
    spatialMap.clear();
    
    const size_t n = particles.count();
    for (size_t i = 0; i < n; i++)
    {
        int cellX = particles.getCellX(i, cellSize);
        int cellY = particles.getCellY(i, cellSize);
        spatialMap[hash(cellX, cellY)].push_back(i);
    }
}

void Sim::getNeighbors(size_t particleIdx, std::vector<size_t>& neighbors) const
{
    neighbors.clear();
    
    int cellX = particles.getCellX(particleIdx, cellSize);
    int cellY = particles.getCellY(particleIdx, cellSize);

    // Zoek in 3x3 grid rondom particle
    for (int dx = -1; dx <= 1; dx++)
    {
        for (int dy = -1; dy <= 1; dy++)
        {
            int64_t h = hash(cellX + dx, cellY + dy);
            auto it = spatialMap.find(h);
            if (it != spatialMap.end())
            {
                for (size_t neighborIdx : it->second)
                {
                    if (neighborIdx != particleIdx)
                        neighbors.push_back(neighborIdx);
                }
            }
        }
    }
}

void Sim::update(float dt)
{
    // Beperk tijdstap voor stabiliteit
    const float maxDt = 0.016f;  // Max 16ms
    if (dt > maxDt) dt = maxDt;

    time = GetTime();
    SetShaderValue(shader, timeLoc, &time, SHADER_UNIFORM_FLOAT);

    // Build spatial hash eenmalig
    buildSpatialMap();

    // Sub-stepping voor betere stabiliteit (minder dan voorheen vanwege fixed-point)
    const int substeps = 2;
    const float subDt = dt / substeps;
    
    for (int step = 0; step < substeps; step++)
    {
        // PARALLEL INTERACTIONS
        const size_t n = particles.count();
        
        if (n > 1000 && threadCount > 1) {
            // Multi-threaded voor grote aantallen
            std::vector<std::thread> threads;
            const size_t chunkSize = n / threadCount;
            
            for (unsigned int t = 0; t < threadCount; t++)
            {
                size_t start = t * chunkSize;
                size_t end = (t == threadCount - 1) ? n : (t + 1) * chunkSize;
                
                threads.emplace_back([this, start, end]() {
                    std::vector<size_t> neighbors;
                    neighbors.reserve(32);
                    
                    for (size_t i = start; i < end; i++)
                    {
                        getNeighbors(i, neighbors);
                        for (size_t j : neighbors)
                        {
                            particles.interact(i, j);
                        }
                    }
                });
            }
            
            for (auto &thread : threads)
                thread.join();
        } else {
            // Single-threaded voor kleine aantallen (minder overhead)
            std::vector<size_t> neighbors;
            neighbors.reserve(32);
            
            for (size_t i = 0; i < n; i++)
            {
                getNeighbors(i, neighbors);
                for (size_t j : neighbors)
                {
                    particles.interact(i, j);
                }
            }
        }
        
        // PHYSICS UPDATE (ultra fast met fixed-point!)
        particles.update(subDt);
        
        // BOUNDS (kan ook parallel maar overhead is vaak niet waard)
        for (size_t i = 0; i < n; i++)
        {
            particles.resolveBounds(glass, i);
        }
        
        // Rebuild spatial map voor volgende substep
        if (step < substeps - 1) {
            buildSpatialMap();
        }
    }
}

void Sim::draw() const
{
    DrawRectangleRounded(rect, ROUNDED, 10, UI::get_dcolor(UiColors::FIRST));
    if (BORDER_WIDTH > 0)
        DrawRectangleRoundedLinesEx(rect, ROUNDED, 10, BORDER_WIDTH, UI::get_dcolor(UiColors::BORDER));
    
    // Render naar texture
    BeginTextureMode(this->win);
    ClearBackground(BLANK);
    
    // Direct draw van alle particles (kan verder geoptimaliseerd met batching)
    particles.draw();
    
    EndTextureMode();

    // Draw texture met shader
    BeginShaderMode(shader);
    DrawTextureRec(
        this->win.texture,
        (Rectangle){0, 0, (float)this->win.texture.width, -(float)this->win.texture.height},
        (Vector2){rect.x + PEDING * 2, rect.y + LINE + PEDING * 2},
        WHITE
    );
    EndShaderMode();
    
    // Debug info (optioneel)
    #ifdef DEBUG
    DrawText(TextFormat("Particles: %zu", particles.count()), 
             (int)rect.x + 10, (int)rect.y + 10, 20, WHITE);
    DrawText(TextFormat("FPS: %d", GetFPS()), 
             (int)rect.x + 10, (int)rect.y + 35, 20, WHITE);
    #endif
}

size_t Sim::getParticleCount() const
{
    return particles.count();
}