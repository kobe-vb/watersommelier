#pragma once

#include "UI.hpp"

#include <sstream>
#include <string>

class TextInp : public UI
{
private:
    bool is_hover;
    bool is_active = false;
    Rectangle bounds;
    std::string text;
    const std::string tmp;
    Color bg_color = GRAY;
    bool on_focus_clear = false;

public:
    TextInp(float x, float y, float w, float h, std::function<void(UI &)> callback = nullptr, const std::string &tmp = "");
    void draw(void) const override;
    void update(void) override;
    std::string &get_text(void);
    const Rectangle &get_rect(void);
    bool capture_tab(void) override;
    void move(int x, int y);
    void reset(void);
    void set_text(const std::string &text);
    void set_on_focus_clear(bool value) { on_focus_clear = value; }

    void remove_active(void);

    void set_bg_color(const Color &color);
};