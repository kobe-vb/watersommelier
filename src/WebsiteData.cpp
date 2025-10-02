#include "WebsiteData.hpp"
#include <sstream>

static std::string urlDecode(const std::string &src)
{
    std::string ret;
    char ch;
    int ii;
    for (size_t i = 0; i < src.length(); i++)
    {
        if (src[i] == '%')
        {
            if (i + 2 < src.length())
            {
                std::istringstream iss(src.substr(i + 1, 2));
                iss >> std::hex >> ii;
                ch = static_cast<char>(ii);
                ret += ch;
                i += 2;
            }
        }
        else if (src[i] == '+')
            ret += ' ';
        else
            ret += src[i];
    }
    return ret;
}

WebsiteData::WebsiteData(std::string &string)
{
    // username=kobe&zuur=2&biter=5&final_coment=llo&_leker=woow+heel+lekr&favorite_water=5&water_preference=Tap+water
    this->innit = true;

    int i = 0;
    std::string key, value;
    std::stringstream ss(string);
    while (std::getline(ss, key, '=') && std::getline(ss, value, '&'))
    {
        key = urlDecode(key);
        value = urlDecode(value);
        if (key[0] == '_' || key[0] == '#')
        {
            this->custemData += value + ";";
            i++;
            continue;
        }
        this->data[key] = value;
    }
    i /= 3;
    custemData = std::to_string(i) + ";" + custemData;
}

std::string WebsiteData::get(std::string key)
{
    auto it = data.find(key);
    if (it == this->data.end())
        return "";
    return it->second;
}

std::string WebsiteData::get_end_data()
{
    return this->custemData;
}