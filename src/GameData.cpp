# include "GameData.hpp"

bool load_data(GameData &data)
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
        
        if (row.size() >= 5) 
        {
           data.opt.push_back(row[0]); // chemical name
            data.code.push_back(row[3]); // bar code
        }
    }
    
    file.close();
    return true;
}