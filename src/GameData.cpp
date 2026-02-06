#include "GameData.hpp"
#include <algorithm>

static float to_float(const std::string &str)
{
    std::string value = str;
    std::replace(value.begin(), value.end(), ',', '.'); // vervang ',' door '.'
    return std::stof(value);
    ;
}

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

static std::string to_lowercase(const std::string &input)
{
    std::string result = input;
    std::transform(result.begin(), result.end(), result.begin(),
                   [](unsigned char c)
                   { return std::tolower(c); });
    return result;
}

static void store_element(GameData &data, std::vector<std::string> row)
{
    try
    {
        if (row.size() >= 8) ///////////////////
        {
            Ion anion(to_lowercase(row[3]), to_float(row[2]));
            Ion kation(to_lowercase(row[5]), to_float(row[4]));
            Element element(anion, kation, to_float(row[6]));
            data.add_element(row[1], row[0], element);
            // std::cout << anion.ion << " : " << anion.mol << std::endl;
        }
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what();
        std::cout << " row: ";
        for (size_t i = 0; i < row.size(); i++)
            std::cout << row[i] << " ";
        std::cout << std::endl;
    }
}

static void store_ion(GameData &data, std::vector<std::string> row)
{
    if (row.size() >= 5)
    {
        int r = std::stoi(row[1]);
        int g = std::stoi(row[2]);
        int b = std::stoi(row[3]);
        Color col = {
            static_cast<unsigned char>(r),
            static_cast<unsigned char>(g),
            static_cast<unsigned char>(b),
            255};

        IonData ion(col, to_float(row[4]), to_float(row[5]));
        data.ions.insert_or_assign(to_lowercase(row[0]), ion);
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

        while (std::getline(ss, cell, ';'))
            row.push_back(cell);
        store_element(data, row);
    }
    file.close();
    return true;
}

static bool load_ions(GameData &data)
{
    std::ifstream file("data/input/ions.csv");
    if (!file.is_open())
    {
        std::cerr << "Error opening ions.csv file" << std::endl;
        return false;
    }

    std::string line;
    std::getline(file, line);

    while (std::getline(file, line))
    {
        std::stringstream ss(line);
        std::string cell;
        std::vector<std::string> row;

        while (std::getline(ss, cell, ';'))
            row.push_back(cell);
        store_ion(data, row);
    }

    file.close();
    return true;
}

bool load_data(GameData &data)
{
    try
    {
        if (!load_elements(data))
            return false;
        return (load_ions(data));
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << std::endl;
        return false;
    }
}