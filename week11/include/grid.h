#pragma once
#include "config.h"
#include <vector>
#include <cstdint>

struct Grid {
    int min_x, max_x;
    int min_y, max_y;
    std::vector<std::vector<uint64_t>> data;
};

Grid loadInitialGrid(const Config& config);
