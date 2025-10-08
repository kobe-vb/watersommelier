#include "BarcodeReader.hpp"

#include <raylib.h>

void BarcodeReader::update()
{
    int key = GetKeyPressed();
    double now = GetTime();

    while (key > 0)
    {
        double delta = now - lastKeyTime;
        lastKeyTime = now;

        if (delta < speedThreshold)
        {
            if (!building)
                buffer.clear();
            building = true;
            buffer += (char)key;
        }
        else
        {
            if (building && !buffer.empty())
                callback(buffer.c_str());
            buffer.clear();
            building = false;
        }

        if (key == KEY_ENTER && building)
        {
            callback(buffer.c_str());
            buffer.clear();
            building = false;
        }

        key = GetKeyPressed();
        now = GetTime();
    }

    if (building && (GetTime() - lastKeyTime) > timeout)
    {
        callback(buffer.c_str());
        buffer.clear();
        building = false;
    }
}
