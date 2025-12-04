#include "ButtonModel.hpp"

ButtonModel::ButtonModel(const std::string &text, std::function<void()> callback) : UIModel(callback), text(text) {}