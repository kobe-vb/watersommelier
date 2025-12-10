#pragma once

#include <string>
#include <functional>
#include <fstream>

class CSVDownloader
{
private:
    std::ofstream file;

    size_t counter = 0;
    bool is_beginning_of_line = true;

public:
    CSVDownloader(const std::string &filename, std::function<std::string (void)> create_header);
    ~CSVDownloader();

    CSVDownloader& operator<<(const std::string &info);
    CSVDownloader& operator<<(const float &info);
    void newline(void);

    size_t get_index(void) const { return counter; }
};