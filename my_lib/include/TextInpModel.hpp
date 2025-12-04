#pragma once

#include "UIModel.hpp"

#include <string>
#include <functional>

class TextInpModel : public UIModel
{
private:
    std::string text;
    const std::string tmp;
    bool on_focus_clear = false;

    bool _hover = false;
    bool _active = false;

public:
    TextInpModel(const std::string &tmp, std::function<void()> callback = nullptr);
    ~TextInpModel() = default;
    TextInpModel(const TextInpModel &);
    TextInpModel &operator=(const TextInpModel &);

    const std::string &get_text(void) const;
    const std::string &get_display_text(void) const;
    void set_text(const std::string &text);
    void add_text(const std::string &text);

    void reset(void);
    void set_on_focus_clear(bool value) { on_focus_clear = value; }

    void set_hover(bool value) { _hover = value; }
    void set_active(bool value) { _active = value; }

    bool is_hover() const { return _hover; }
    bool is_active() const { return _active; }
    bool is_empty() const { return text.empty(); }

    void focus(void);
    void add_char(char c);
    void backspace(void);
};