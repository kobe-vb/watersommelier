#include "PlayerModel.hpp"
#include "GameModel.hpp"

PlayerModel::PlayerModel(const std::string &name, GameData &data, Sim &sim, GameModel *game) : name(name),
                                                              glass(GlassModel(data, [this]()
                                                                          { score_glass(); }, sim)), scoreGlass([this](){next_glass();}, [this](){return steal_glass();}), game(game)
{
    scoreGlass.disable();
    website.disable();
}

void PlayerModel::demo(void)
{
    int max = std::rand() % 6 + 4;
    for (int i = 0; i < max; i++)
    {
        glass.generate_random_data();
        history.saveGlass(i, glass, scoreGlass);
        glass.reset();
    }
    glass.generate_random_data(false);
    // glass.reset_sim();
}

// TODO: refactor
void PlayerModel::save_data(std::ofstream &file, size_t &counter, GameData &data)
{
    if (history.get_num_of_elements() == 0)
        return;
    history.save_data(file, counter, name, data, website.get_data());
}

// TODO: refactor
std::string PlayerModel::to_json() const
{
    std::stringstream json;
    json << "{\n";
    json << "  \"name\": \"" << name << "\",\n";
    json << "  \"glasses\": [\n";

    for (int i = 0; i < history.get_num_of_elements(); i++)
    {
        if (i > 0)
            json << ",\n";
        const HistoryGlass &glass = dynamic_cast<const HistoryGlass &>(*history.get_ui_at(i));
        json << glass.to_json();
    }

    json << "\n  ]\n}";
    return json.str();
}

void PlayerModel::fiks_sim(void)
{
    glass.reset_sim();
}

void PlayerModel::score_glass()
{
    glass.lock();
    scoreGlass.activate();
}

void PlayerModel::next_glass()
{
    if (thief)
    {
        thief->save_stolen_glass(glass, scoreGlass);
        thief = nullptr;
    }
    else
        history.saveGlass(glass.get_id(), glass, scoreGlass);
    glass.reset();
    scoreGlass.reset();
    scoreGlass.disable();
}

bool PlayerModel::steal_glass()
{
    if (thief)
    {
        thief->save_stolen_glass(glass, scoreGlass);
        thief = nullptr;
    }
    else
        history.saveGlass(glass.get_id(), glass, scoreGlass);
    
    const std::string &key = scoreGlass.get_thief();
    std::cout << "steal glass for " << key << "\n";
    thief = game->get_player(key);
    if (!thief || thief == this)
    {
        thief = nullptr;
        return false;
    }
    scoreGlass.reset();
    return true;
}

void PlayerModel::save_stolen_glass(GlassModel &glass, ScoreGlassModel &scoreGlass)
{
    history.saveGlass(this->glass.get_id(), glass, scoreGlass);
    this->glass.inc();
}

bool PlayerModel::is_my_code(const std::string &code) const
{
    return (code == this->code);
}

bool PlayerModel::take_code_for_dropdown(const std::string &code)
{
    return glass.take_code(code);
}

bool PlayerModel::set_code(const std::string &new_code)
{
    if (code.length() == 0)
    {
        this->code = new_code;
        return true;
    }
    return false;
}

void PlayerModel::set_data(std::string website_data)
{
    website.set_data(website_data);
}

void PlayerModel::set_website(std::string website)
{
    glass.disable();
    scoreGlass.disable();
    this->website.activate();

    this->website.set_website(website);
}

void PlayerModel::update()
{
    history.update();
}

