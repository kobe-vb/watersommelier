#include "SpatialGrid.hpp"
#include <cmath>

SpatialGrid::SpatialGrid(float cellSz, int screenW, int screenH)
    : cellSize(cellSz)
{
    gridWidth = (int)(screenW / cellSize) + 1;
    gridHeight = (int)(screenH / cellSize) + 1;
}

int SpatialGrid::getCellID(Vector2 pos) const
{
    int cx = (int)(pos.x / cellSize);
    int cy = (int)(pos.y / cellSize);
    cx = fmaxf(0, fminf(gridWidth - 1, cx));
    cy = fmaxf(0, fminf(gridHeight - 1, cy));
    return cx + cy * gridWidth;
}

void SpatialGrid::clear()
{
    cells.clear();
}

void SpatialGrid::insert(int ballIdx, Vector2 pos)
{
    int cellID = getCellID(pos);
    cells[cellID].push_back(ballIdx);
}

std::vector<int> SpatialGrid::queryNeighbors(Vector2 pos) const
{
    std::vector<int> neighbors;

    int cx = (int)(pos.x / cellSize);
    int cy = (int)(pos.y / cellSize);

    for (int dy = -1; dy <= 1; dy++)
    {
        for (int dx = -1; dx <= 1; dx++)
        {
            int checkX = cx + dx;
            int checkY = cy + dy;

            if (checkX < 0 || checkX >= gridWidth ||
                checkY < 0 || checkY >= gridHeight)
                continue;

            int cellID = checkX + checkY * gridWidth;

            auto it = cells.find(cellID);
            if (it != cells.end())
            {
                neighbors.insert(neighbors.end(),
                                 it->second.begin(),
                                 it->second.end());
            }
        }
    }

    return neighbors;
}