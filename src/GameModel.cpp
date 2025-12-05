
#include "GameModel.hpp"

#include <fstream>
#include <cstdint>
#include <filesystem>
#include <SudoPlayerModel.hpp>

// TODO: refactor
static void saveCounter(size_t counter, const std::string &filename)
{
    std::ofstream out(filename, std::ios::binary);
    if (!out)
    {
        std::cerr << "Failed to open file for writing: " << filename << std::endl;
        return;
    }
    out.write(reinterpret_cast<const char *>(&counter), sizeof(counter));
}

// TODO: refactor
static size_t loadCounter(const std::string &filename)
{
    std::ifstream in(filename, std::ios::binary);
    if (!in)
        return 1;
    size_t counter;
    in.read(reinterpret_cast<char *>(&counter), sizeof(counter));
    return counter;
}

GameModel::GameModel()
{
    barcode_reader = BarcodeReader([this](const std::string &code)
                                   { handleCode(code); });

    // TODO: refactor
    load_data(data);
}

// TODO: refactor
void GameModel::save_data(void)
{
    size_t counter = loadCounter("data/output/counter.bin");

    bool file_exists = std::filesystem::exists("data/output/data.csv");
    std::ofstream file("data/output/data.csv", std::ios::app);
    if (file.is_open())
    {
        if (!file_exists)
        {
            file << "i;datum;name;SsC Zoet; SsC Zout; SsC zuur; SsC Bitter; SsC Umami;comment;end comment;osmo;Tot volume(ml);";
            for (auto &ion : data.ions)
                file << ion.first << " %" << ";" << ion.first << " mol" << ";" << ion.first << " mg" << ";";
            file << "hastags;" << std::endl;
        }

        // for (auto &player : players)
        // {
        //     if (!DEBUG && (player.player->get_name().starts_with("demo") || player.player->get_name().starts_with("web")))
        //         continue;
        //     player.player->save_data(file, counter, data);
        // }
        file.close();
    }
    saveCounter(counter, "data/output/counter.bin");
}

int GameModel::get_active_player(void)
{
    if (players.size() == 0)
        return -1;
    return active_player;
}

void GameModel::switch_players(int id)
{
    for (int i = 0; i < (int)players.size(); i++)
    {
        if (i == id)
        {
            get_player(i)->activate();
            get_tokel(i).set_tokel(true);
        }
        else
        {
            get_tokel(i).set_tokel(false);
            get_player(i)->disable();
        }
    }
    active_player = id;

    // std::cout << "switch_players id: " << id << std::endl;
    // active_player = id;
    // if (id < 0)
    // {
    //     reset_sim();
    //     return;
    // }
    // if (id >= (int)players.size())
    // {
    //     std::cerr << "Error: player id out of range: " << id << std::endl;
    //     return;
    // }
    // players[id]->fiks_sim();
}

void GameModel::reset_sim(void)
{
    sim.set_rect(); // //////////////////////
    // sim.reset();
}

std::vector<PlayerModel *> GameModel::get_players_ref()
{
    std::vector<PlayerModel *> refs;
    refs.reserve(players.size());

    for (auto &p : players)
        refs.push_back(p.get());

    return refs;
}

int GameModel::create_player()
{
    const std::string &name = name_input.get_text();

    if (name == "tiboon")
        players.push_back(std::make_unique<SudoPlayerModel>(name, data, sim, get_players_ref()));
    else
        players.push_back(std::make_unique<PlayerModel>(name, data, sim));

    int id = players.size() - 1;

    buttons.push_back(std::make_unique<TokelModel>(name, [this, id]()
                                                   { switch_players(id); }));
    return id;
}

bool GameModel::name_is_taken(const std::string &name)
{
    for (auto &player : players)
    {
        if (player->get_name() == name)
            return (true);
    }
    return (false);
}

void GameModel::handleCode(const std::string &code)
{
    std::cout << "code: \"" << code << "\"" << std::endl;

    // let the active player handle the code
    if (players.size() > 0 && active_player >= 0 && players[active_player]->take_code_for_dropdown(code))
        return;
    // switch players
    for (auto &player : players)
    {
        if (player->is_my_code(code))
        {
            // playerWrapper.tokel->set_tokel(true);
            return;
        }
    }
    if (players.size() == 0 || active_player < 0)
        return;
    // set the code to the active player
    players[active_player]->set_code(code);
}

bool GameModel::update(void)
{
    this->sim.update(GetFrameTime());
    barcode_reader.update();
    if (barcode_reader.isBuilding())
        return false;
    return true;
}
