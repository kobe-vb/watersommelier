#include "CSVDownloader.hpp"

#include <cstdint>
#include <filesystem>
#include <iostream>

static void saveCounter(size_t counter, const std::string &filename)
{
    std::ofstream out(filename, std::ios::binary);
    if (!out)
    {
        std::cerr << "Failed to open file for writing: " << filename << std::endl;
        return;
    }
    out.write(reinterpret_cast<const char *>(&counter), sizeof(counter));
}

static size_t loadCounter(const std::string &filename)
{
    std::ifstream in(filename, std::ios::binary);
    if (!in)
        return 1;
    size_t counter;
    in.read(reinterpret_cast<char *>(&counter), sizeof(counter));
    return counter;
}

CSVDownloader::CSVDownloader(const std::string &filename, std::function<std::string (void)> create_header)
{
    counter = loadCounter("data/output/counter.bin"); // file name?

    bool file_exists = std::filesystem::exists(filename);
    file = std::ofstream(filename, std::ios::app);

    if (!file.is_open())
    {
        std::cerr << "Failed to open file for writing: " << filename << std::endl;
        return;
    }
        if (!file_exists)
        {
            file << create_header() << std::endl;
        }
}

CSVDownloader::~CSVDownloader()
{
    saveCounter(counter, "data/output/counter.bin"); // file name?
    file.close();
}

CSVDownloader& CSVDownloader::operator<<(const std::string &info)
{
    if (file.is_open())
    {
        if (is_beginning_of_line)
        {
            file << counter;
            is_beginning_of_line = false;
        }
        file << ";" << info;
    }
    return *this;
}

CSVDownloader& CSVDownloader::operator<<(const float &info)
{
    return (*this << std::to_string(info));
}


void CSVDownloader::newline(void)
{
    if (file.is_open())
    {
        file << std::endl;
        counter++;
        is_beginning_of_line = true;
    }
}

