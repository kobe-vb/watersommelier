#include "PauseView.hpp"
#include <cmath>
#include <iostream>

PauseView::PauseView()
{
    logo = LoadTexture("data/img/logo.png");
}

PauseView::~PauseView()
{
    UnloadTexture(logo);
}

void PauseView::update(void)
{
    time += GetFrameTime();
}

void PauseView::draw(void) const
{
    int screenW = GetScreenWidth();
    int screenH = GetScreenHeight();

    DrawRectangle(0, 0, screenW, screenH, Fade(BLACK, 0.8f));

    // --- WIEBEL EFFECT ---
    float wobble = sinf(time * 3.0f) * 10.0f;   // horizontale beweging
    float scalePulse = 1.0f + sinf(time * 2.0f) * 0.05f; // lichte schaal puls

    // --- Logo centreren ---
    float logoWidth  = logo.width  * scalePulse * 0.6f;
    float logoHeight = logo.height * scalePulse * 0.6f;
    std::cout << logoWidth << " " << logoHeight << std::endl;

    float logoX = screenW / 2.0f - logoWidth / 2.0f;
    float logoY = screenH / 2.0f - logoHeight / 2.0f + 200;

    Rectangle src = { 0, 0, (float)logo.width, (float)logo.height };
    Rectangle dest = { logoX, logoY, logoWidth, logoHeight };
    Vector2 origin = { 0, 0 };

    DrawTexturePro(logo, src, dest, origin, 0.0f, WHITE);

    // --- Tekst erboven ---
    const char* text = "JOW VERGEET U NIET TE SCANNEN";
    int fontSize = 40;

    int textWidth = MeasureText(text, fontSize);

    DrawText(
        text,
        screenW / 2 - textWidth / 2 + wobble,
        logoY - 220,
        fontSize,
        YELLOW
    );
}
