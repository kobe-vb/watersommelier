#include "Sim.hpp"
#include <raylib.h>
#include <Settings.hpp>
#include <UIView.hpp>
#include <cmath>
#include <cassert>

// Boids parameters - experimenteer hiermee!
const float SEPARATION_RADIUS = 25.0f;
const float ALIGNMENT_RADIUS = 50.0f;
const float COHESION_RADIUS = 50.0f;

const float SEPARATION_FORCE = 1.5f;
const float ALIGNMENT_FORCE = 0.8f;
const float COHESION_FORCE = 0.8f;

const float GRAFFITI_force = 0.5f;

const float MAX_SPEED = 150.0f;
const float MAX_FORCE = 200.0f;

Sim::Sim()
{
    set_rect();

    std::cout << "rect: " << rect.width << ", " << rect.height << std::endl;

    win = LoadRenderTexture(rect.width - PEDING * 2, rect.height - PEDING * 2);
    SetTextureFilter(win.texture, TEXTURE_FILTER_BILINEAR);

    shader = LoadShader(0, "data/shaders/water.fs");

    SetShaderValue(shader,
                   GetShaderLocation(shader, "resolution"),
                   (float[2]){(float)win.texture.width, (float)win.texture.height},
                   SHADER_UNIFORM_VEC2);

    // Data texture
    Image img = GenImageColor(MAX_BALLS, 1, BLACK);
    dataTex = LoadTextureFromImage(img);
    colorTex = LoadTextureFromImage(img);
    UnloadImage(img);
    SetTextureFilter(dataTex, TEXTURE_FILTER_POINT);
    SetTextureWrap(dataTex, TEXTURE_WRAP_CLAMP);

    // Initialize spatial grid
    spatialGrid = SpatialGrid(COHESION_RADIUS, win.texture.width, win.texture.height);

    // Add boid swarms
    addParticles(100, RED);
    addParticles(100, GREEN);
    addParticles(100, BLUE);
    addParticles(100, YELLOW);
    addParticles(100, PURPLE);

    // addParticles(10, RED);
    // addParticles(10, GREEN);
    // addParticles(10, BLUE);
    // addParticles(10, YELLOW);
    // addParticles(10, PURPLE);
}

Sim::~Sim()
{
    UnloadRenderTexture(win);
    UnloadShader(shader);
    UnloadTexture(dataTex);
}

void Sim::set_rect()
{
    rect.width = ((GetScreenWidth() - (UI_BORDER * 4) - (PEDING * 2)) * 1 / 3);
    rect.height = GetScreenHeight() - LINE - PEDING - UI_BORDER;
    rect.x = UI_BORDER * 2 + (rect.width + PEDING) * 2;
    rect.y = LINE;
}

void Sim::reset()
{
    balls.clear();
    Image img = GenImageColor(MAX_BALLS, 1, BLACK);
    UpdateTexture(dataTex, img.data);
}

void Sim::addParticles(float n, Color col)
{
    for (int i = 0; i < (int)n && balls.size() < MAX_BALLS; i++)
    {
        Metaball b;
        b.pos = {(float)GetRandomValue(50, win.texture.width - 50),
                 (float)GetRandomValue(50, win.texture.height - 50)};
        
        // Random richting maar lagere snelheid
        float angle = GetRandomValue(0, 360) * DEG2RAD;
        float speed = GetRandomValue(30, 60);
        b.vel = {cosf(angle) * speed, sinf(angle) * speed};
        
        b.color = col;
        balls.push_back(b);
    }

    std::vector<Color> colors(MAX_BALLS, BLACK);
    for (int i = 0; i < (int)balls.size(); i++)
    {
        colors[i] = balls[i].color;
    }
    UpdateTexture(colorTex, colors.data());
}

// Helper functie om kleur te vergelijken
bool colorsMatch(Color a, Color b)
{
    return a.r == b.r && a.g == b.g && a.b == b.b;
}

// Vector2 helper functies
Vector2 normalize(Vector2 v)
{
    float len = sqrtf(v.x * v.x + v.y * v.y);
    if (len > 0.0f)
        return {v.x / len, v.y / len};
    return {0, 0};
}

Vector2 limit(Vector2 v, float max)
{
    float len = sqrtf(v.x * v.x + v.y * v.y);
    if (len > max)
    {
        v.x = (v.x / len) * max;
        v.y = (v.y / len) * max;
    }
    return v;
}

void Sim::update(float dt)
{
    spatialGrid.clear();
    for (size_t i = 0; i < balls.size(); i++)
    {
        spatialGrid.insert(i, balls[i].pos);
    }

    std::vector<Vector2> accelerations(balls.size(), {0, 0});

    for (size_t i = 0; i < balls.size(); i++)
    {
        auto &boid = balls[i];
        
        // Zoek neighbors met spatial grid
        std::vector<int> neighbors = spatialGrid.queryNeighbors(boid.pos);

        Vector2 separation = {0, 0};
        Vector2 alignment = {0, 0};
        Vector2 cohesion = {0, 0};
        
        int sepCount = 0;
        int aliCount = 0;
        int cohCount = 0;

        for (int idx : neighbors)
        {
            if (idx == (int)i) continue;
            
            auto &other = balls[idx];
            Vector2 diff = {boid.pos.x - other.pos.x, boid.pos.y - other.pos.y};
            float dist = sqrtf(diff.x * diff.x + diff.y * diff.y);
            
            if (dist < 0.1f) continue; // Te dichtbij
            
            // Kleur bonus - zelfde kleur = sterkere interactie
            bool sameColor = colorsMatch(boid.color, other.color);

            // SEPARATION - vermijd dichtbij neighbors
            if (dist < SEPARATION_RADIUS)
            {
                Vector2 repel = normalize(diff);
                repel.x /= dist; // Sterker als dichterbij
                repel.y /= dist;
                separation.x += repel.x;
                separation.y += repel.y;
                sepCount++;
            }

            // ALIGNMENT - match snelheid van nearby boids (vooral eigen kleur)
            if (sameColor && dist < ALIGNMENT_RADIUS)
            {
                alignment.x += other.vel.x;
                alignment.y += other.vel.y;
                aliCount++;
            }

            // COHESION - beweeg naar centrum van groep (vooral eigen kleur)
            if (sameColor && dist < COHESION_RADIUS)
            {
                cohesion.x += other.pos.x;
                cohesion.y += other.pos.y;
                cohCount++;
            }
        }

        // Bereken gemiddelden en steer forces
        Vector2 steer = {0, 0};

        if (sepCount > 0)
        {
            separation.x /= sepCount;
            separation.y /= sepCount;
            separation = normalize(separation);
            separation.x *= MAX_SPEED;
            separation.y *= MAX_SPEED;
            
            steer.x = separation.x - boid.vel.x;
            steer.y = separation.y - boid.vel.y;
            steer = limit(steer, MAX_FORCE);
            
            accelerations[i].x += steer.x * SEPARATION_FORCE;
            accelerations[i].y += steer.y * SEPARATION_FORCE;
        }

        if (aliCount > 0)
        {
            alignment.x /= aliCount;
            alignment.y /= aliCount;
            alignment = normalize(alignment);
            alignment.x *= MAX_SPEED;
            alignment.y *= MAX_SPEED;
            
            steer.x = alignment.x - boid.vel.x;
            steer.y = alignment.y - boid.vel.y;
            steer = limit(steer, MAX_FORCE);
            
            accelerations[i].x += steer.x * ALIGNMENT_FORCE;
            accelerations[i].y += steer.y * ALIGNMENT_FORCE;
        }

        if (cohCount > 0)
        {
            cohesion.x /= cohCount;
            cohesion.y /= cohCount;
            
            Vector2 desired = {cohesion.x - boid.pos.x, cohesion.y - boid.pos.y};
            desired = normalize(desired);
            desired.x *= MAX_SPEED;
            desired.y *= MAX_SPEED;
            
            steer.x = desired.x - boid.vel.x;
            steer.y = desired.y - boid.vel.y;
            steer = limit(steer, MAX_FORCE);
            
            accelerations[i].x += steer.x * COHESION_FORCE;
            accelerations[i].y += steer.y * COHESION_FORCE;
        }
    }

    // Apply forces en update posities
    float r = 10.0f; // Border margin
    for (size_t i = 0; i < balls.size(); i++)
    {
        auto &b = balls[i];
        
        // Update velocity
        b.vel.x += accelerations[i].x * dt;
        b.vel.y += accelerations[i].y * dt;

        b.vel.y += GRAFFITI_force;
        
        // Limit speed
        b.vel = limit(b.vel, MAX_SPEED);
        
        
        // Update position
        b.pos.x += b.vel.x * dt;
        b.pos.y += b.vel.y * dt;

        // Wrap around borders (optioneel: kan ook bounce zijn)
        if (b.pos.x < r)
        {
            b.pos.x = r;
            b.vel.x *= -0.5f; // Dempen bij botsing
        }
        else if (b.pos.x > win.texture.width - r)
        {
            b.pos.x = win.texture.width - r;
            b.vel.x *= -0.5f;
        }

        if (b.pos.y < r)
        {
            b.pos.y = r;
            b.vel.y *= -0.5f;
        }
        else if (b.pos.y > win.texture.height - r)
        {
            b.pos.y = win.texture.height - r;
            b.vel.y *= -0.5f;
        }
    }

    updateShaderData();
}

void Sim::updateShaderData()
{
    int n = balls.size();
    if (n > MAX_BALLS)
        n = MAX_BALLS;

    std::vector<Color> pixels(MAX_BALLS, BLACK);
    
    for (int i = 0; i < n; i++)
    {
        float normX = balls[i].pos.x / win.texture.width;
        float normY = balls[i].pos.y / win.texture.height;
        
        int xInt = (int)(normX * 65535.0f);
        int yInt = (int)(normY * 65535.0f);
        
        pixels[i].r = (xInt >> 8) & 0xFF;
        pixels[i].g = xInt & 0xFF;
        pixels[i].b = (yInt >> 8) & 0xFF;
        pixels[i].a = yInt & 0xFF;
    }

    UpdateTextureRec(dataTex, Rectangle{0, 0, (float)n, 1}, pixels.data());
}

void Sim::draw() const
{
    DrawRectangleRounded(rect, ROUNDED, 10, UIView::get_dcolor(UiColors::FIRST));
    if (BORDER_WIDTH > 0)
        DrawRectangleRoundedLinesEx(rect, ROUNDED, 10,
                                    BORDER_WIDTH, UIView::get_dcolor(UiColors::BORDER));

    int count = balls.size();
    if (count > MAX_BALLS)
        count = MAX_BALLS;

    SetShaderValueTexture(shader, GetShaderLocation(shader, "ballData"), dataTex);
    SetShaderValueTexture(shader, GetShaderLocation(shader, "ballColors"), colorTex);

    SetShaderValue(shader,
                   GetShaderLocation(shader, "ballCount"),
                   &count,
                   SHADER_UNIFORM_INT);

    BeginTextureMode(win);
    ClearBackground(BLANK);
    EndTextureMode();

    BeginShaderMode(shader);
    DrawTexturePro(
        win.texture,
        {0, 0, (float)win.texture.width, -(float)win.texture.height},
        {rect.x + PEDING, rect.y + PEDING, rect.width - PEDING * 2, rect.height - PEDING * 2},
        {0, 0},
        0.0f,
        WHITE);
    EndShaderMode();

#ifdef DEBUG
    DrawText(TextFormat("Boids: %zu", balls.size()),
             rect.x + 10, rect.y + 10, 20, WHITE);
#endif
}