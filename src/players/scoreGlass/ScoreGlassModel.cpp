#include "ScoreGlassModel.hpp"
#include "Settings.hpp"
#include <regex>

ScoreGlassModel::ScoreGlassModel(std::function<void()> next_glass, std::function<bool()> steal_glass) : next_glass(next_glass), steal_glass(steal_glass)
{
    if (DEBUG)
        comment_input.set_text("vdf #one #two cool #three, #four #five, #six #seven");
}

void ScoreGlassModel::add_score(void)
{
    const std::string &comment = this->comment_input.get_text();
    std::regex hashtagRegex("#\\w+");

    auto it = std::sregex_iterator(comment.begin(), comment.end(), hashtagRegex);
    auto end = std::sregex_iterator();

    for (; it != end; ++it)
    {
        TextInpModel inp = TextInpModel(it->str(), nullptr);
        if (DEBUG)
            inp.set_text("5");
        hastags.emplace(it->str(), inp);
    }

}

int ScoreGlassModel::get_hastag_value(const TextInpModel &inp) const
{
    int value = -1;
    try
    {
        value = std::stoi(inp.get_text());
        if (value <= 0 || value > 5)
            return -1;
    }
    catch (...)
    {
        return value;
    }
    return value;
}

std::map<std::string, int> ScoreGlassModel::get_hastags(void) const
{
    std::map<std::string, int> res;
    for (const auto &it : this->hastags)
        res[it.first] = get_hastag_value(it.second);
    return res;
}
