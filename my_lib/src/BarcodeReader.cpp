#include "BarcodeReader.hpp"

#include <raylib.h>
#include <algorithm>
#include <iostream>

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
            {
                runCallback();
                continue; // TODO: ik denk dat ge kunt returnen
            }
            buffer.clear();
            building = false;
        }

        if (key == KEY_ENTER && building)
            runCallback();

        key = GetKeyPressed();
        now = GetTime();
    }

    if (building && (GetTime() - lastKeyTime) > timeout)
        runCallback();
    // std::cout << "barcode reader code: \"" << buffer << "\"\n";
}

void BarcodeReader::cleanBuffer(void)
{
    // remove non-printable characters
    buffer.erase(std::remove_if(buffer.begin(), buffer.end(),
                                [](unsigned char c)
                                { return !std::isprint(c); }),
                 buffer.end());

    // Trim spaces at the beginning and end
    buffer.erase(0, buffer.find_first_not_of(' '));
    buffer.erase(buffer.find_last_not_of(' ') + 1);
}

void BarcodeReader::runCallback(void)
{

    cleanBuffer();
    callback(buffer);
    buffer.clear();
    building = false;
}
