#include "PlayerModel.hpp"

PlayerModel::PlayerModel(const std::string &name, GameData &data, Sim &sim) : name(name),
                                                              glass(GlassModel(data, [this]()
                                                                          { score_glass(); }, sim)), scoreGlass([this](){next_glass();}, [this](){return steal_glass();})
{
}

void PlayerModel::demo(void)
{
    int max = std::rand() % 6 + 4;
    for (int i = 0; i < max; i++)
    {
        // glass.generate_random_data();
        history.saveGlass(glass, scoreGlass);
        glass.reset();
    }
    // glass.generate_random_data(false);
    glass.reset_sim();
}

// TODO: refactor
void PlayerModel::save_data(std::ofstream &file, size_t &counter, GameData &data)
{
    if (history.get_num_of_elements() == 0)
        return;
    history.save_data(file, counter, name, data, website_data);
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
}

void PlayerModel::next_glass()
{
    history.saveGlass(glass, scoreGlass);
    glass.reset();
}

bool PlayerModel::steal_glass()
{
    std::cout << "steal glass\n";
    return false;
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

void PlayerModel::update()
{
    history.update();
}

// TODO: refactor
void PlayerModel::set_website(std::string website)
{
    this->website = website;
    this->qr = qrcodegen::QrCode::encodeText(website.c_str(), qrcodegen::QrCode::Ecc::LOW);
    // this->qr = qrcodegen::QrCode::encodeText("WIFI:T:WPA;S:yourAP;P:yourPassword;;", qrcodegen::QrCode::Ecc::LOW);

    /*
    this->add_ui(std::make_unique<Button>(
        rect.x + 50, rect.y + 500, 200, 50, "Go to website",
        [url = this->website](UI& ui)
        {
            (void)ui;
            std::thread([url]()
            {
                // std::string command = "start \"\" \"" + url + "\"";
                std::string command = "start " + url;
                system(command.c_str());
            }).detach();
        }));
        */
}
// TODO: refactor
void PlayerModel::set_website_data(std::string website_data)
{
    this->website_data = WebsiteData(website_data);
}