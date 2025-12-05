#pragma once

#include <functional>

class UIModel
{

private:
    std::function<void()> callback;

    bool locked = false;
    bool visible = true;

public:
    UIModel(std::function<void()> callback = nullptr) : callback(callback) {}
    ~UIModel() = default;

    void set_callback(std::function<void()> new_callback) { callback = new_callback; }
    std::function<void()> get_callback(void) const { return callback; }
    void run_callback(void)
    {
        if (callback)
            callback();
    }

    void lock(void) { locked = true; }
    void unlock(void) { locked = false; }
    void set_locked(bool value) { locked = value; }
    bool is_locked(void) const { return locked; }

    void set_visible(bool value) { visible = value; }
    bool is_visible(void) const { return visible; }

    void activate(void)
    {
        locked = false;
        visible = true;
    }
    void disable(void)
    {
        locked = true;
        visible = false;
    }
};