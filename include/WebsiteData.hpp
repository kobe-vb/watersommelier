#pragma once

#include <map>
#include <string>

class WebsiteData
{
    private:
        std::map<std::string, std::string> data;
        std::string custemData;
        bool innit = false;
    public:
        WebsiteData() = default;
        WebsiteData(std::string &string);
        std::string get(std::string key);
        const std::string &get_end_data() const;
        bool empty() const { return !this->innit; }
        void set_begin_index(int begin_index);
};