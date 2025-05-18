


#pragma once

#include "BufferedWin.hpp"
#include "Glass.hpp"
#include "raylib.h"

class HistoryGlass : public Glass {
    public:
        HistoryGlass(GameData &data, std::function<void(UI &)> close_glas);
        ~HistoryGlass() = default;
        void draw(void) const override;
};