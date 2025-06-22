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

    void remove_active(void);
};