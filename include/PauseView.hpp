#pragma once
#include <raylib.h>

class PauseView 
{
public:
    PauseView();
    ~PauseView();

    void update(void);
    void draw(void) const;

private:
    Texture2D logo;
    float time = 0.0f;
};
