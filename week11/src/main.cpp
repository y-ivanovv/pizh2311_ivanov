#include "config.h"
#include "grid.h"
#include "sandpile.h"
#include "bmp_writer.h"
#include <filesystem>
#include <iostream>

namespace fs = std::filesystem;

int main(int argc, char** argv) {
    Config config = parseArgs(argc, argv);
    fs::create_directories(config.output_dir);

    Grid grid = loadInitialGrid(config);
    bool stable = false;
    uint64_t iter = 0;

    while (iter < config.max_iter && !stable) {
        bool hasToppled = false;
        Grid new_grid = nextIteration(grid, hasToppled);
        if (!hasToppled) stable = true;

        if (config.freq > 0 && (iter % config.freq == 0 || iter == config.max_iter - 1 || stable)) {
            std::string path = config.output_dir + "/iter_" + std::to_string(iter) + ".bmp";
            saveBMP(new_grid, path, iter);
        }

        grid = new_grid;
        iter++;
    }

    if (config.freq == 0 && !stable) {
        saveBMP(grid, config.output_dir + "/final.bmp", iter);
    }

    return 0;
}
