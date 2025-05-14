#pragma once
#include <string>
#include <cstdint>

struct Config {
    uint16_t length;
    uint16_t width;
    std::string input_file;
    std::string output_dir;
    uint64_t max_iter;
    uint64_t freq;
};

Config parseArgs(int argc, char** argv);