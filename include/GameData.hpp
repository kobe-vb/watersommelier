#pragma once

#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

struct GameData
{
    std::vector<std::string> opt;
    std::vector<std::string> code;
};

bool load_data(GameData &data);
