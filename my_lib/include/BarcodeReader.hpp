#pragma once

#include <string>
#include <functional>

class BarcodeReader
{
    private:
    std::string buffer;
    double lastKeyTime = 0;
    bool building = false;
    double timeout = 0.1;         // 100ms stilte = einde barcode
    double speedThreshold = 0.01; // <50ms tussen toetsen = scanner

    std::function<void(const std::string &)> callback;
    void cleanBuffer(void);
    void runCallback(void);

    public:

        BarcodeReader() = default;
        BarcodeReader(std::function<void(const std::string &)> callback) : callback(callback) {}
        void update();
        bool isBuilding(void) { return building; }
};
