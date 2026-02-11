#include "ActivityTracker.hpp"

#include "raylib.h"

ActivityTracker::ActivityTracker()
{
    lastActivityTime = GetTime();
}

void ActivityTracker::update()
{
    if (detect_activity())
        lastActivityTime = GetTime();
}

float ActivityTracker::seconds_since_activity() const
{
    return static_cast<float>(GetTime() - lastActivityTime);
}

bool ActivityTracker::detect_activity() const
{
    if (GetKeyPressed() != 0)
        return true;

    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) ||
        IsMouseButtonPressed(MOUSE_RIGHT_BUTTON) ||
        IsMouseButtonPressed(MOUSE_MIDDLE_BUTTON))
        return true;

    Vector2 delta = GetMouseDelta();
    if (delta.x != 0 || delta.y != 0)
        return true;

    return false;
}
