# pragma once

#include "Win.hpp"
#include "Button.hpp"
#include "Tokel.hpp"

class Glass: public Win
{
private:
    int ind;
public:
    Glass(int ind);
    ~Glass() = default;

    void draw(void) const override;
};