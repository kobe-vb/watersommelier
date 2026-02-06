#pragma once
#include <raylib.h>
#include <vector>
#include "SpatialGrid.hpp"

#define MAX_BALLS 1000

struct Metaball
{
    Vector2 pos;
    Vector2 vel;
    Color color;
};

class Sim
{
public:
    Sim();
    ~Sim();
    void update(float dt);
    void draw() const;
    void reset();
    void addParticles(float n, Color col);

private:
    void set_rect();
    void updateShaderData();

    Rectangle rect;
    RenderTexture2D win;
    Shader shader;
    Texture2D dataTex;
    Texture2D colorTex;
    
    std::vector<Metaball> balls;
    SpatialGrid spatialGrid;
};