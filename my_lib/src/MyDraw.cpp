#include "MyDraw.hpp"

std::unordered_map<std::string, std::unordered_map<int, Font>> MyDraw::data;
std::string MyDraw::path;

void MyDraw::init(const std::string &path)
{
    MyDraw::path = path;
}

void MyDraw::destroy(void)
{
    for (auto it = MyDraw::data.begin(); it != MyDraw::data.end(); it++)
    {
        for (auto it2 = it->second.begin(); it2 != it->second.end(); it2++)
            UnloadFont(it2->second);
    }
    MyDraw::data.clear();
}

void MyDraw::text(const std::string &font, const std::string &text, float x, float y, int size, Color color)
{
    if (MyDraw::data.find(font) == MyDraw::data.end())
        MyDraw::data[font] = std::unordered_map<int, Font>();
    if (MyDraw::data[font].find(size) == MyDraw::data[font].end())
        MyDraw::data[font][size] = LoadFontEx((path + font + ".ttf").c_str(), size, 0, 0);
    DrawTextEx(MyDraw::data[font][size], text.c_str(), Vector2{x, y}, size, 5, color);
}