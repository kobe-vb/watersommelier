

# pragma once

#include "GameData.hpp"
#include "Win.hpp"
#include "GlassModel.hpp"
#include "ScoreGlassModel.hpp"
#include "HistoryGlass.hpp"
#include "raylib.h"
#include "WebsiteData.hpp"
#include "CSVDownloader.hpp"

class History: public Win
{
private:

    UIModel model;

    float scrollOffset;    
    float maxScrollOffset;
    Rectangle rect;

    void calculateMaxScroll(void);
    bool updateScroll(void);
    
public:

    History(void);
    ~History(void) = default;

    void saveGlass(int id, GlassModel &glass, ScoreGlassModel &scoreGlass);

    bool update(void) override;
    void draw(void) const override;

    void save_data(CSVDownloader &csv, const std::string &name, GameData &data, WebsiteData &websiteData);
};

