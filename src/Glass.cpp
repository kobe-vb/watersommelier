
#include "Glass.hpp"

void draw_my_text(const char *name, float val, int x, int y)
{
    char buffer[64];
    snprintf(buffer, sizeof(buffer), name, val);
    DrawText(buffer, x, y, 30, DARKBLUE);
}

Glass::Glass(int ind, GameData &data, std::function<void(UI &)> close_glas) : ind(ind), data(data), close_glas_func(close_glas)
{
    x = 100;
    y = 200 + ind * 165;

    rect = {static_cast<float>(x), static_cast<float>(y), 1500, 70};
    add_ui(std::make_unique<Dropdown>(
        x, y + 10, 400, 50,
        data.names, "lol"));
    add_ui(std::make_unique<TextInp>(x + 410, y + 10, 100, 50, [this](UI &ui)
                                  { save_druple(ui); }, "hoeveel?"));
    add_ui(std::make_unique<Button>(x + 520, y + 10, 100, 50, "save",
                                    [this](UI &ui)
                                    { save_druple(ui); }));
    add_ui(std::make_unique<Button>(x + 1400, y + 10, 100, 50, "save?",
                                    [this](UI &ui)
                                    { add_comment(ui); }));
    bar = StackedBar(x + 630, y + 10, 400, 50);
    // bar.add_value("lol", {255, 0, 0, 255}, 5);
    // bar.add_value("dirk", {255, 255, 0, 255}, 10);
    // bar.add_value("dirk", {255, 255, 0, 255}, 10);
    name = (Dropdown *)get_ui_at(0);
    amount = (TextInp *)get_ui_at(1);
}

void Glass::save_druple(UI &ui)
{
    int a;
    Element *elm;
    try
    {
        a = std::stoi(amount->get_text());
        elm = &data.get_element(name->get_selected_text());
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << '\n';
        return;
    }
    bar.add_value(elm->anion.ion, data.get_color(elm->anion.ion), elm->anion.mol * a);
    bar.add_value(elm->kation.ion, data.get_color(elm->kation.ion), elm->kation.mol * a);

    ph += elm->ph * a;
    mol += elm->mol * a;
    
    name->reset();
    amount->reset();
}

void Glass::add_comment(UI &ui)
{
    _is_active = false;
    add_ui(std::make_unique<TextInp>(x + 20, y + 80, 500, 50, close_glas_func, "coment?"));
    
    rect.height += 70;
}

void Glass::draw(void) const
{
    DrawRectangleRounded(rect, 0.2, 8, LIGHTGRAY);
    DrawRectangleRoundedLinesEx(rect, 0.2, 8, 6.0f, BLACK);
    draw_my_text("pH: %.2f", ph, x + 1050, y + 10);
    draw_my_text("mol: %.2f", mol, x + 1180, y + 10);
    bar.draw();
    Win::draw();
}
