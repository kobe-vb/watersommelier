/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Particle.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kvanden- <kvanden-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/15 17:23:39 by kvanden-          #+#    #+#             */
/*   Updated: 2025/11/14 00:00:00 by kvanden-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#include <raylib.h>
#include <vector>
#include <cstdint>
#include <cmath>

// Fixed-point particle systeem - ultra efficient!
// Gebruikt 16-bit integers voor positie en snelheid
class ParticleSystem {
public:
    // Fixed-point scales
    static constexpr int POS_SCALE = 4;        // Position: 4x scale (0-2400 voor 600px)
    static constexpr int VEL_SCALE = 256;      // Velocity: 8 bits fractional
    static constexpr float RADIUS = 2.0f;
    
    // Physics constants (pre-calculated in fixed-point waar mogelijk)
    static constexpr int16_t GRAVITY_FIXED = (int16_t)(150.0f * VEL_SCALE / 60.0f);  // Per frame @ 60fps
    static constexpr int16_t DAMPING_NUM = 255;     // 255/256 ≈ 0.998
    static constexpr int16_t DAMPING_SHIFT = 8;
    // static constexpr int32_t MAX_SPEED2 = (int32_t)(200.0f * 200.0f * VEL_SCALE * VEL_SCALE);
    
    // Boid constants
    static constexpr int16_t COHESION_RANGE_MIN = 15 * POS_SCALE;
    static constexpr int16_t COHESION_RANGE_MAX = 40 * POS_SCALE;
    static constexpr int16_t SEPARATION_RANGE = 15 * POS_SCALE;
    static constexpr int16_t ALIGNMENT_RANGE = 50 * POS_SCALE;
    static constexpr int16_t REPULSION_RANGE = 12 * POS_SCALE;
    
private:
    // Structure of Arrays voor maximale cache efficiency
    alignas(32) std::vector<int16_t> posX;
    alignas(32) std::vector<int16_t> posY;
    alignas(32) std::vector<int16_t> velX;
    alignas(32) std::vector<int16_t> velY;
    std::vector<uint32_t> colors;  // Packed RGBA
    
public:
    ParticleSystem() {
        reserve(10000);  // Pre-allocate voor minder reallocaties
    }
    
    void reserve(size_t n) {
        posX.reserve(n);
        posY.reserve(n);
        velX.reserve(n);
        velY.reserve(n);
        colors.reserve(n);
    }
    
    size_t count() const { return posX.size(); }
    
    void clear() {
        posX.clear();
        posY.clear();
        velX.clear();
        velY.clear();
        colors.clear();
    }
    
    void addParticle(float x, float y, float vx, float vy, Color col) {
        posX.push_back((int16_t)(x * POS_SCALE));
        posY.push_back((int16_t)(y * POS_SCALE));
        velX.push_back((int16_t)(vx * VEL_SCALE));
        velY.push_back((int16_t)(vy * VEL_SCALE));
        
        // Pack color in 1 uint32
        uint32_t packed = ((uint32_t)col.r << 24) | 
                         ((uint32_t)col.g << 16) | 
                         ((uint32_t)col.b << 8) | 
                         col.a;
        colors.push_back(packed);
    }
    
    // Ultra fast physics update - pure integer math!
    inline void update(float dt)
    {
        (void)dt;
        // const size_t n = count();
        // const int16_t gravity = (int16_t)(GRAVITY_FIXED * dt * 60.0f);
        
        // // Deze loop wordt geoptimaliseerd naar SIMD met -O3 -march=native
        // for (size_t i = 0; i < n; i++) {
        //     // Gravity (integer add)
        //     velY[i] += gravity;
            
        //     // Damping: multiply by 255/256
        //     velX[i] = (velX[i] * DAMPING_NUM) >> DAMPING_SHIFT;
        //     velY[i] = (velY[i] * DAMPING_NUM) >> DAMPING_SHIFT;
            
        //     // Speed limiting (alleen als nodig - branch prediction friendly)
        //     int32_t speed2 = (int32_t)velX[i] * velX[i] + (int32_t)velY[i] * velY[i];
        //     if (speed2 > MAX_SPEED2) {
        //         float speed = sqrtf((float)speed2);
        //         float factor = (200.0f * VEL_SCALE) / speed;
        //         velX[i] = (int16_t)(velX[i] * factor);
        //         velY[i] = (int16_t)(velY[i] * factor);
        //     }
            
        //     // Position update (bit shifts zijn super snel)
        //     posX[i] += velX[i] >> (VEL_SCALE / POS_SCALE);
        //     posY[i] += velY[i] >> (VEL_SCALE / POS_SCALE);
        // }
    }
    
    // Bounds checking
    inline void resolveBounds(const Rectangle& bounds, size_t i) {
        const int16_t minX = (int16_t)(bounds.x * POS_SCALE);
        const int16_t maxX = (int16_t)((bounds.x + bounds.width) * POS_SCALE);
        const int16_t minY = (int16_t)(bounds.y * POS_SCALE);
        const int16_t maxY = (int16_t)((bounds.y + bounds.height) * POS_SCALE);
        const int16_t radius = (int16_t)(RADIUS * POS_SCALE);
        
        // X bounds
        if (posX[i] < minX + radius) {
            posX[i] = minX + radius;
            velX[i] = -(velX[i] * 7) / 10;  // -0.7 damping
        } else if (posX[i] > maxX - radius) {
            posX[i] = maxX - radius;
            velX[i] = -(velX[i] * 7) / 10;
        }
        
        // Y bounds
        if (posY[i] < minY + radius) {
            posY[i] = minY + radius;
            velY[i] = -(velY[i] * 7) / 10;
        } else if (posY[i] > maxY - radius) {
            posY[i] = maxY - radius;
            velY[i] = -(velY[i] * 7) / 10;
        }
    }
    
    // Boid interaction tussen twee particles
    inline void interact(size_t i, size_t j) {
        int32_t dx = (int32_t)posX[j] - posX[i];
        int32_t dy = (int32_t)posY[j] - posY[i];
        int32_t dist2 = dx * dx + dy * dy;
        
        // Early exit als te ver weg
        if (dist2 > (int32_t)ALIGNMENT_RANGE * ALIGNMENT_RANGE) return;
        
        // Check of zelfde kleur (vloeistof)
        bool sameColor = colors[i] == colors[j];
        
        if (sameColor) {
            // Zelfde vloeistof - boid gedrag
            if (dist2 > 1 && dist2 < (int32_t)ALIGNMENT_RANGE * ALIGNMENT_RANGE) {
                int16_t dist = (int16_t)sqrtf((float)dist2);
                
                // Cohesie - trek naar elkaar
                if (dist > COHESION_RANGE_MIN && dist < COHESION_RANGE_MAX) {
                    int16_t forceX = (dx * 8) / dist;  // Strength ≈ 0.8 in fixed point
                    int16_t forceY = (dy * 8) / dist;
                    
                    velX[i] += forceX >> 3;
                    velY[i] += forceY >> 3;
                }
                // Separation - duw weg
                else if (dist < SEPARATION_RANGE && dist > 0) {
                    int16_t strength = ((SEPARATION_RANGE - dist) * 2) / SEPARATION_RANGE;
                    int16_t forceX = (dx * strength) / dist;
                    int16_t forceY = (dy * strength) / dist;
                    
                    velX[i] -= forceX >> 4;
                    velY[i] -= forceY >> 4;
                }
                
                // Alignment - match velocity
                if (dist < ALIGNMENT_RANGE) {
                    int16_t avgVelX = (velX[i] + velX[j]) >> 1;
                    int16_t avgVelY = (velY[i] + velY[j]) >> 1;
                    
                    velX[i] += (avgVelX - velX[i]) >> 5;  // 0.05 strength
                    velY[i] += (avgVelY - velY[i]) >> 5;
                }
            }
            
            // Turbulence voor levendigheid
            if (dist2 < 900 * POS_SCALE * POS_SCALE) {
                velX[i] += (rand() % 100 - 50);  // Random -50 tot +50
                velY[i] += (rand() % 100 - 50);
            }
        } else {
            // Verschillende vloeistof - repulsie
            if (dist2 < (int32_t)REPULSION_RANGE * REPULSION_RANGE && dist2 > 1) {
                int16_t dist = (int16_t)sqrtf((float)dist2);
                int16_t strength = ((REPULSION_RANGE - dist) * 3 * VEL_SCALE) / REPULSION_RANGE;
                
                int16_t forceX = (dx * strength) / (dist * VEL_SCALE);
                int16_t forceY = (dy * strength) / (dist * VEL_SCALE);
                
                velX[i] -= forceX;
                velY[i] -= forceY;
            }
        }
    }
    
    // Getters voor rendering
    inline Vector2 getPosition(size_t i) const {
        return Vector2{
            (float)posX[i] / POS_SCALE,
            (float)posY[i] / POS_SCALE
        };
    }
    
    inline Color getColor(size_t i) const {
        uint32_t packed = colors[i];
        return Color{
            (unsigned char)(packed >> 24),
            (unsigned char)(packed >> 16),
            (unsigned char)(packed >> 8),
            (unsigned char)packed
        };
    }
    
    // Voor spatial hashing
    inline int getCellX(size_t i, float cellSize) const {
        return posX[i] / (int16_t)(cellSize * POS_SCALE);
    }
    
    inline int getCellY(size_t i, float cellSize) const {
        return posY[i] / (int16_t)(cellSize * POS_SCALE);
    }
    
    // Batch draw voor rendering
    void draw() const {
        const size_t n = count();
        for (size_t i = 0; i < n; i++) {
            DrawCircleV(getPosition(i), RADIUS, getColor(i));
        }
    }
};