#include "TokelModel.hpp"


TokelModel::TokelModel(const std::string &text, std::function<void()> callback) :
    UIModel(callback), text(text)
{}

void TokelModel::set_tokel(bool value)
{
    active = value;
    if (value)
        run_callback();
}