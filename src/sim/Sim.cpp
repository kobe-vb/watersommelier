#include "Sim.hpp"
#include <raylib.h>
#include <Settings.hpp>
#include <UIView.hpp>
#include <cmath>
#include <cassert>

Sim::Sim()
{
    set_rect();

    std::cout << "rect: " << rect.width << ", " << rect.height << std::endl;

    win = LoadRenderTexture(rect.width - PEDING * 2, rect.height - PEDING * 2);
    SetTextureFilter(win.texture, TEXTURE_FILTER_BILINEAR); // Voeg dit toe

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

    // Add some particles
    addParticles(10, RED);
    addParticles(10, GREEN);
    addParticles(10, BLUE);
    addParticles(10, YELLOW);

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
    // clear texture
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
        b.vel = {(float)GetRandomValue(-80, 80),
                 (float)GetRandomValue(-80, 80)};
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

void Sim::update(float dt)
{
    float r = 2.0f;
    for (auto &b : balls)
    {
        b.pos.x += b.vel.x * dt;
        b.pos.y += b.vel.y * dt;

        if (b.pos.x < r)
        {
            b.pos.x = r;
            b.vel.x *= -1;
        }
        else if (b.pos.x > win.texture.width - r)
        {
            b.pos.x = win.texture.width - r;
            b.vel.x *= -1;
        }

        if (b.pos.y < r)
        {
            b.pos.y = r;
            b.vel.y *= -1;
        }
        else if (b.pos.y > win.texture.height - r)
        {
            b.pos.y = win.texture.height - r;
            b.vel.y *= -1;
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

    // SetShaderValueTexture(shader, dataTexLoc, dataTex);
    SetShaderValueTexture(shader, GetShaderLocation(shader, "ballData"), dataTex);
    SetShaderValueTexture(shader, GetShaderLocation(shader, "ballColors"), colorTex);

    SetShaderValue(shader,
                   GetShaderLocation(shader, "ballCount"),
                   &count,
                   SHADER_UNIFORM_INT);

    // data texture
    // hier hoeft geen SetShaderValue, shader gebruikt sampler2D uniform
    // Binden gebeurt automatisch bij DrawTexturePro

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
    DrawText(TextFormat("Metaballs: %zu", balls.size()),
             rect.x + 10, rect.y + 10, 20, WHITE);
#endif
}
