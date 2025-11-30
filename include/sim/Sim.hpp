/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Sim.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kvanden- <kvanden-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/15 17:20:57 by kvanden-          #+#    #+#             */
/*   Updated: 2025/11/14 00:00:00 by kvanden-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <raylib.h>
#include <unordered_map>
#include <vector>
#include <cstdint>
#include "Particle.hpp"

class Sim {
private:
    ParticleSystem particles;
    
    // Spatial hashing - nu met indices ipv pointers
    std::unordered_map<int64_t, std::vector<size_t>> spatialMap;
    float cellSize;
    
    // Rendering
    RenderTexture2D win;
    Shader shader;
    int timeLoc;
    float time;
    
    // Boundaries
    Rectangle glass;
    Rectangle rect;
    
    // Multi-threading
    unsigned int threadCount;
    
    // Helper functions
    int64_t hash(int x, int y) const;
    void buildSpatialMap();
    void getNeighbors(size_t particleIdx, std::vector<size_t>& neighbors) const;

public:
    Sim(void);
    ~Sim(void);
    
    void reset(void);
    void set_rect(void);
    void addParticles(float n, Color col);
    void update(float dt);
    void draw() const;
    size_t getParticleCount() const;
};