
# include "Glass.hpp"

Glass::Glass(int ind): ind(ind)
{
    add_ui(std::make_unique<Tokel>(
        10, 200 + ind * 100, 100, 50, "lol"));   
    add_ui(std::make_unique<Tokel>(
        100, 200 + ind * 100, 100, 50, "lol"));
    add_ui(std::make_unique<Tokel>(
        300, 200 + ind * 100, 100, 50, "lol"));   
}

void Glass::draw(void) const
{
    DrawRectangle(500, 200 + ind * 100, 300, 200, RED);
    Win::draw();
}