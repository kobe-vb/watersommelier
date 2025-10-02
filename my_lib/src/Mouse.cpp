#include "Mouse.hpp"
#include "raylib.h"

int Mouse::cursor = MOUSE_CURSOR_DEFAULT;

void Mouse::set_cursor()
{
    static int old_cursor = 0;

    if (old_cursor == cursor)
        return;
    old_cursor = cursor;
    SetMouseCursor(cursor);
}