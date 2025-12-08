#include "SudoPlayerView.hpp"
#include "SudoPlayerModel.hpp"
#include "TokelView.hpp"
#include "Settings.hpp"

SudoPlayerView::SudoPlayerView(PlayerModel *model) : PlayerView(model), _model(static_cast<SudoPlayerModel *>(model))
{
    players_info_rect.width = GetScreenWidth() - (UI_BORDER * 4) - PEDING - rect.width;

    int x = players_info_rect.x + players_info_rect.width + PEDING;
    int y = players_info_rect.y + PEDING * 0.5;
    int w = PLAYER_WIDTH * 0.8;
    int h = PLAYER_HEIGHT;

    int i = 0;
    for (auto &[key, value] : _model->get_buttons())
    {
        add_ui(std::make_unique<TokelView>(&value, x + ((i % 5) * (PEDING + w)), y + ((i / 5) * (PEDING + h)), w, h));
        i++;

        // value.set_callback([this, key]() { this->set_player_website(key); });
    }

    x_text = rect.x + rect.width + PEDING + (rect.width - MeasureText("Role: Sudo", 80)) / 2;
}

// weg ?
void SudoPlayerView::set_player_website(const std::string &name)
{
    std::cout << "Setting website for player: " << name << std::endl;
    _model->set_player_website(name);
}

bool SudoPlayerView::update(void)
{
    if (_model->is_locked())
        return false;
    PlayerView::update();

    return false;
}

void SudoPlayerView::draw(void) const
{
    if (!_model->is_visible())
        return;
    PlayerView::draw();

    DrawText("Role: Sudo", x_text, 170, 80, BLACK);
}
