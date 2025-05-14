#include "grid.h"
#include "config.h"
#include <iostream>
#include <fstream>
#include <sstream>

Grid loadInitialGrid(const Config& config) {
    Grid grid;
    grid.min_x = 0;
    grid.max_x = config.width - 1;
    grid.min_y = 0;
    grid.max_y = config.length - 1;
    grid.data.resize(config.length, std::vector<uint64_t>(config.width, 0));

    std::ifstream file(config.input_file);
    std::string line;
    while (std::getline(file, line)) {
        std::istringstream iss(line);
        uint16_t x, y;
        uint64_t value;
        if (!(iss >> x >> y >> value)) {
            std::cerr << "Invalid input format" << std::endl;
            exit(1);
        }
        if (x >= config.width || y >= config.length) {
            std::cerr << "Initial coordinates out of bounds" << std::endl;
            exit(1);
        }
        grid.data[y][x] = value;
    }
    return grid;
}