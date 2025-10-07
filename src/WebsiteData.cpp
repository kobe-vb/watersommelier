#include "WebsiteData.hpp"
#include <sstream>
#include <iostream>
#include <regex>

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
    i /= 5;
    custemData = std::to_string(i) + ";" + custemData;
    std::cout << custemData << std::endl;
}

std::string WebsiteData::get(std::string key)
{
    auto it = data.find(key);
    if (it == this->data.end())
        return "";
    return it->second;
}

void WebsiteData::set_begin_index(int begin_index)
{
    // "2;#bitter;glass 1;glass 3;A_gt;2;#salty;glass 4;glass 5;B_gt;1;"

    std::regex glass_pattern(R"(glass\s+(\d+))");
    std::smatch match;
    std::string result;
    std::string::const_iterator searchStart(custemData.cbegin());

    while (std::regex_search(searchStart, custemData.cend(), match, glass_pattern))
    {
        result.append(match.prefix().first, match.prefix().second);

        int original_num = std::stoi(match[1]);
        int new_num = original_num + begin_index - 1;
        result += "glass " + std::to_string(new_num);

        searchStart = match.suffix().first;
    }
    result.append(searchStart, custemData.cend());

    this->custemData = result;
}

const std::string &WebsiteData::get_end_data() const
{
    return this->custemData;
}