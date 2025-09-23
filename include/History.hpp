

# pragma once

#include "GameData.hpp"
#include "Win.hpp"
#include "Glass.hpp"
#include "HistoryGlass.hpp"
#include "raylib.h"

class History: public Win
{
private:
    float scrollOffset;    
    float maxScrollOffset;
    Rectangle rect;

    void calculateMaxScroll(void);
    bool updateScroll(void);
    
public:

    History(void);
    ~History(void) = default;

    void saveGlass(Glass &glass);

    void update(void) override;
    void draw(void) const override;

    void save_data(std::ofstream &file, size_t &counter, const std::string &name, GameData &data);
};

