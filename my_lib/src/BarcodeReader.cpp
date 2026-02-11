#include "BarcodeReader.hpp"
#include <raylib.h>
#include <cctype>

BarcodeReader::BarcodeReader(Callback cb, double timeoutSeconds)
    : callback(std::move(cb)), timeout(timeoutSeconds)
{
}

void BarcodeReader::update()
{
    int ch = GetCharPressed();
    double now = GetTime();

    while (ch > 0)
    {
        lastInputTime = now;

        if (ch == '\r' || ch == '\n')
        {
            runCallback();
        }
        else if (std::isprint(ch))
        {
            buffer += static_cast<char>(ch);
        }

        ch = GetCharPressed();
        now = GetTime();
    }

    flushIfNeeded();
}

void BarcodeReader::flushIfNeeded()
{
    if (!buffer.empty())
    {
        double now = GetTime();
        if ((now - lastInputTime) > timeout)
        {
            runCallback();
        }
    }
}

void BarcodeReader::runCallback()
{
    if (buffer.empty())
        return;

    callback(buffer);
    buffer.clear();
}

void BarcodeReader::clear()
{
    buffer.clear();
}
