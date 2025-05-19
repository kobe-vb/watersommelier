

# pragma once

#include "Win.hpp"
#include "Glass.hpp"
#include "HistoryGlass.hpp"
#include "raylib.h"

class History: public Win
{
private:

    Rectangle rect;
    
public:

    History(void);
    ~History(void) = default;

    void saveGlass(Glass &glass);

    void draw(void) const override;
};

