#pragma once

#include <functional>

class UIModel
{

private:
    std::function<void()> callback;

public:

    UIModel(std::function<void()> callback = nullptr) : callback(callback) {}
    ~UIModel() = default;

    void set_callback(std::function<void()> new_callback) { callback = new_callback; }
    std::function<void()> get_callback(void) const { return callback; }
    void run_callback(void) { if (callback) callback(); }
};