#pragma once

#include "qrcodegen.hpp"
#include <optional>

#include "WebsiteModel.hpp"
#include "Win.hpp"
#include "Settings.hpp"

class WebsiteView : public Win
{

private:
    int qr_size;
    int size;
    int offset_x;
    int offset_y;

    WebsiteModel *model;

    std::optional<qrcodegen::QrCode> qr;
    Rectangle rect;

public:
    WebsiteView(WebsiteModel *model);
    ~WebsiteView() = default;

    bool update(void) override;
    void draw(void) const override;
};