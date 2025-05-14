#include "sandpile.h"
#include "grid.h"
#include <vector>
#include <map>

const std::vector<std::pair<int, int>> directions = { {0, -1}, {0, 1}, {-1, 0}, {1, 0} };

Grid nextIteration(const Grid& grid, bool& hasToppled) {
    std::map<std::pair<int, int>, uint64_t> temp;
    hasToppled = false;

    for (int y = 0; y < grid.data.size(); ++y) {
        for (int x = 0; x < grid.data[y].size(); ++x) {
            int x_real = grid.min_x + x;
            int y_real = grid.min_y + y;
            uint64_t grains = grid.data[y][x];
            if (grains >= 4) {
                hasToppled = true;
                uint64_t remaining = grains - 4;
                temp[{x_real, y_real}] += remaining;
                for (const auto& [dx, dy] : directions) {
                    int nx = x_real + dx;
                    int ny = y_real + dy;
                    temp[{nx, ny}]++;
                }
            }
            else {
                temp[{x_real, y_real}] += grains;
            }
        }
    }

    if (!hasToppled) return grid;

    int new_min_x = grid.min_x, new_max_x = grid.max_x;
    int new_min_y = grid.min_y, new_max_y = grid.max_y;
    for (const auto& [coord, _] : temp) {
        new_min_x = std::min(new_min_x, coord.first);
        new_max_x = std::max(new_max_x, coord.first);
        new_min_y = std::min(new_min_y, coord.second);
        new_max_y = std::max(new_max_y, coord.second);
    }

    Grid new_grid;
    new_grid.min_x = new_min_x;
    new_grid.max_x = new_max_x;
    new_grid.min_y = new_min_y;
    new_grid.max_y = new_max_y;
    int new_width = new_max_x - new_min_x + 1;
    int new_height = new_max_y - new_min_y + 1;
    new_grid.data.resize(new_height, std::vector<uint64_t>(new_width, 0));

    for (const auto& [coord, value] : temp) {
        int x = coord.first - new_grid.min_x;
        int y = coord.second - new_grid.min_y;
        new_grid.data[y][x] = value;
    }

    return new_grid;
}