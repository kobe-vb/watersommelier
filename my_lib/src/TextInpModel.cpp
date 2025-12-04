#include "TextInpModel.hpp"

TextInpModel::TextInpModel(const std::string &tmp, std::function<void()> callback) : UIModel(callback), tmp(tmp) {}

TextInpModel::TextInpModel(const TextInpModel &other) : UIModel(other.get_callback()), text(other.text), tmp(other.tmp) {}

TextInpModel &TextInpModel::operator=(const TextInpModel &other)
{
    UIModel::operator=(other);
    text = other.text;
    // tmp = other.tmp;
    return (*this);
}

void TextInpModel::focus(void)
{
    set_active(!is_active());
    if (is_active() && on_focus_clear)
        text.clear();
}

void TextInpModel::add_char(char key)
{
    if (is_active())
        if (key >= 32 && key <= 126)
            text += key;
}

void TextInpModel::backspace(void)
{
    if (is_active() && !text.empty())
        text.pop_back();
}

void TextInpModel::set_text(const std::string &text)
{
    this->text = text;
}

const std::string &TextInpModel::get_text(void) const
{
    return (text);
}

void TextInpModel::add_text(const std::string &text)
{
    this->text += text;
}

const std::string &TextInpModel::get_display_text(void) const
{
    return ((is_empty() && !is_active()) ? tmp : text);
}

void TextInpModel::reset(void)
{
    text.clear();
}

