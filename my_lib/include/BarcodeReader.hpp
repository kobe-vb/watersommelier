#pragma once

#include <string>
#include <functional>

class BarcodeReader
{
public:
    using Callback = std::function<void(const std::string&)>;

    explicit BarcodeReader(Callback cb, double timeoutSeconds = 0.3);

    void update();
    void clear();

private:
    void flushIfNeeded();
    void runCallback();

    std::string buffer;
    Callback callback;

    double lastInputTime = 0.0;
    double timeout;
};
