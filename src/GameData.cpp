#include "GameData.hpp"

template <typename T>
std::ostream &operator<<(std::ostream &os, const std::vector<T> &vec)
{
    os << "[";
    for (size_t i = 0; i < vec.size(); ++i)
    {
        os << vec[i];
        if (i != vec.size() - 1)
            os << ", ";
    }
    os << "]";
    return os;
}

static void store_element(GameData &data, std::vector<std::string> row)
{
    if (row.size() >= 8) ///////////////////
    {
        Ion anion(row[5], std::stof(row[6]));
        Ion kation(row[7], std::stof(row[8]));
        Element element(anion, kation, 1, 1);
        data.add_element(row[0], row[3], element);
        // std::cout << anion.ion << " : " << anion.mol << std::endl;
    }
}

static bool load_elements(GameData &data)
{
    std::ifstream file("data/input/data.csv");
    if (!file.is_open())
    {
        std::cerr << "Error opening data.csv file" << std::endl;
        return false;
    }

    std::string line;
    std::getline(file, line);

    while (std::getline(file, line))
    {
        std::stringstream ss(line);
        std::string cell;
        std::vector<std::string> row;

        while (std::getline(ss, cell, ','))
            row.push_back(cell);
        store_element(data, row);
    }
    file.close();
    return true;
}

static bool load_colors(GameData &data)
{
    std::ifstream file("data/input/colors.csv");
    if (!file.is_open())
    {
        std::cerr << "Error opening colors.csv file" << std::endl;
        return false;
    }

    std::string line;

    while (std::getline(file, line))
    {
        std::stringstream ss(line);
        std::string cell;
        std::vector<std::string> row;

        while (std::getline(ss, cell, ','))
        {
            cell.erase(0, cell.find_first_not_of(" \t"));
            cell.erase(cell.find_last_not_of(" \t") + 1);
            row.push_back(cell);
        }

        if (row.size() == 5)
        {
            const std::string &ion = row[0];
            int r = std::stoi(row[1]);
            int g = std::stoi(row[2]);
            int b = std::stoi(row[3]);
            int a = std::stoi(row[4]);
            data.ion_color_map[ion] = {
                static_cast<unsigned char>(r),
                static_cast<unsigned char>(g),
                static_cast<unsigned char>(b),
                static_cast<unsigned char>(a)};
        }
        else
            std::cerr << "Invalid color row: " << line << std::endl;
    }
    file.close();
    return true;
}

bool load_data(GameData &data)
{
    if (!load_elements(data))
        return false;
    return (load_colors(data));
}