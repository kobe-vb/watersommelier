#pragma once

#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <unordered_map>

#include "raylib.h"

#define VOLUME_DRUPEL 0.000055

struct Ion
{
    std::string name;
    int n;
    Ion(const std::string &name, int m) : name(name), n(m) {}
};

struct Element // zout
{
    Ion anion;
    Ion kation;
    double m;

    Element(const Ion &a, const Ion &k, double m)
        : anion(a), kation(k), m(m) {}
};

struct IonData
{
    Color color;
    float gram_per_mol;
    float max_mol_per_glass;

    IonData(Color c, float a, float m) : color(c), gram_per_mol(a), max_mol_per_glass(m) {}
};

struct GameData
{
    std::vector<std::string> names;
    std::vector<std::string> codes;
    std::vector<Element> elements;
    std::unordered_map<std::string, IonData> ions;

    std::unordered_map<std::string, int> name_to_element_map;

    const IonData &get_ion_data(const std::string &ion) const
    {
        auto it = ions.find(ion);
        if (it != ions.end())
            return it->second;
        throw std::invalid_argument("Ion not found");
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
