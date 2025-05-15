/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Sim.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kvanden- <kvanden-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/14 17:26:01 by kvanden-          #+#    #+#             */
/*   Updated: 2025/05/15 18:16:41 by kvanden-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#include "Particle.hpp"
#include <unordered_map>
#include <vector>
#include <cmath>

class Sim
{
public:

    Sim(void);
    ~Sim(void) = default;

    void addParticles(float n, Color col);
    void update(float dt);
    void draw(void) const;

private:

    float cellSize = 20.0f;
    std::vector<Particle> particles;
    using Cell = std::vector<Particle *>;
    std::unordered_map<int64_t, Cell> spatialMap;

private:

    int64_t hash(int x, int y) const;
    void insertIntoSpatialMap(Particle &p);
    std::vector<Particle *> getNeighbors(const Particle &p);
};
