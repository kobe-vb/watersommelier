#pragma once

#include "qrcodegen.hpp"
#include <optional>

#include "WebsiteData.hpp"

#include "UIModel.hpp"
#include "ButtonModel.hpp"
#include "ButtonView.hpp"

// TODO: better naming
class WebsiteModel : public UIModel
{

private:
    std::string website;
    WebsiteData data;

public:
    WebsiteModel();
    ~WebsiteModel() = default;

    ButtonModel website_button;

    void set_website(std::string website);
    void set_data(std::string website_data);

    WebsiteData &get_data(void) { return data; }

    const std::string &get_website(void) const { return website; }
};