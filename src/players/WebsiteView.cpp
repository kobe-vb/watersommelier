#include "WebsiteView.hpp"
#include "Settings.hpp"
#include <MyDraw.hpp>

WebsiteView::WebsiteView(WebsiteModel *model) : Win(model), model(model)
{
    rect.x = UI_BORDER * 2 + PEDING;
    rect.y = LINE + PEDING;
    rect.width = ((GetScreenWidth() - (UI_BORDER * 4) - (PEDING * 2)) * 1 / 3) - 2 * PEDING;
    int delta = rect.width * 0.2;
    rect.width -= delta;
    rect.height = rect.width;

    rect.x += delta / 2;

    add_ui(std::make_unique<ButtonView>(&model->website_button, rect.x + rect.width / 2, rect.y + rect.height + PEDING, BUTTON_WIDTH, BUTTON_HEIGHT, true));
}

bool WebsiteView::update(void)
{
    if (!model->is_visible())
        return (false);

    if (!qr.has_value() && !model->get_website().empty())
    {
        qr = qrcodegen::QrCode::encodeText(model->get_website().c_str(), qrcodegen::QrCode::Ecc::MEDIUM);

        qr_size = qr->getSize();
        int w = rect.width - (PEDING * 2);
        size = w / qr_size;

        int real_width = qr_size * size;
        int delta = (rect.width - real_width);

        rect.width = real_width + (PEDING * 2);
        rect.height = rect.width;
        rect.x += delta / 2;

        offset_x = rect.x + PEDING;
        offset_y = rect.y + PEDING;
    }
    return (Win::update());
}

void WebsiteView::draw(void) const
{
    if (!model->is_visible())
        return;

    if (model->get_data().is_innit())
    {
        MyDraw::text("first", "thx for participating!", rect.x - PEDING, rect.y, 50, get_color(UiColors::TEXT));
        return;
    }
    Win::draw();

    DrawRectangleRounded(rect, ROUNDED, 10, WHITE);

    if (qr.has_value())
    {
        for (int y = 0; y < qr_size; y++)
        {
            for (int x = 0; x < qr_size; x++)
            {
                if (qr->getModule(x, y))
                {
                    DrawRectangle(offset_x + x * size, offset_y + y * size, size, size, BLACK);
                }
            }
        }
    }
}