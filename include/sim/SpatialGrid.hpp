#pragma once

#include <unordered_map>
#include <vector>

#include "raylib.h"

class SpatialGrid
{
private:
    float cellSize;
    int gridWidth;
    int gridHeight;
    std::unordered_map<int, std::vector<int>> cells;

    int getCellID(Vector2 pos) const;

public:
    SpatialGrid() = default;
    SpatialGrid(float cellSz, int screenW, int screenH);
    ~SpatialGrid() = default;

    void init(float cellSz, int screenW, int screenH) { *this = SpatialGrid(cellSz, screenW, screenH); }

    void clear();

    void insert(int ballIdx, Vector2 pos);
    std::vector<int> queryNeighbors(Vector2 pos) const;
};