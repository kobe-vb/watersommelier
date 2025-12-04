#pragma once

#include <string>
#include <functional>

#include "UIModel.hpp"

class ButtonModel: public UIModel
{
private:
    bool _is_hover;
    std::string text;

    public:

    ButtonModel() = default;
    ButtonModel(const std::string &text, std::function<void()> callback = nullptr);
    ~ButtonModel() = default;

    void set_text(const std::string &text) { this->text = text; }
    const std::string &get_text(void) const { return text; }
    const bool &is_hover(void) const { return _is_hover; }
    void set_hover(bool is_hover) { _is_hover = is_hover; }
};