#pragma once

#include <vector>
#include <raylib.h>

#define MAX_BALLS 500  
#define SCALE 1.0f

struct Metaball {
    Vector2 pos;
    Vector2 vel;
    Color color;
};

class Sim {
private:
    RenderTexture2D win;
    Shader shader;
    Texture2D dataTex;
    Texture2D colorTex;
    Rectangle rect;
    std::vector<Metaball> balls;

    void updateShaderData();

public:
    Sim();
    ~Sim();

    void set_rect();
    void reset();
    void addParticles(float n, Color col);
    void update(float dt);
    void draw() const;
};
