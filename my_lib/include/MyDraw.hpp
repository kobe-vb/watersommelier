#pragma once

#include <unordered_map>
#include <string>
#include "raylib.h"

class MyDraw
{
private:
    static std::unordered_map<std::string, std::unordered_map<int, Font>> data;
    static std::string path;
public:
    static void init(const std::string &path);
    static void destroy(void);
    static void text(const std::string &font, const std::string &txt, float x, float y, int size, Color color);
    
};