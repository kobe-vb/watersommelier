#pragma once

#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <unordered_map>

#include "raylib.h"

struct Ion
{
    std::string ion;
    float mol;
    Ion(const std::string &i, float m) : ion(i), mol(m) {}
};

struct Element
{
    Ion anion;
    Ion kation;
    float mol;
    float ph;

    Element(const Ion &a, const Ion &k, float m, float p)
        : anion(a), kation(k), mol(m), ph(p) {}
};

struct GameData
{
    std::vector<std::string> names;
    std::vector<std::string> codes;
    std::vector<Element> elements;
    std::unordered_map<std::string, Color> ion_color_map;
    std::unordered_map<std::string, size_t> name_to_element_map;

    Color get_color(const std::string &ion) const
    {
        auto it = ion_color_map.find(ion);
        if (it != ion_color_map.end())
        {
            return it->second;
        }
        return RED;
    }

    Element &get_element(const std::string &name)
    {
        auto it = name_to_element_map.find(name);
        if (it != name_to_element_map.end())
        {
            return elements[it->second];
        }
        throw std::invalid_argument("Element not found");
    }

    void add_element(const std::string &name, const std::string &code, const Element &element)
    {
        names.push_back(name);
        codes.push_back(code);
        elements.push_back(element);
        name_to_element_map[name] = elements.size() - 1;
    }
};

bool load_data(GameData &data);
