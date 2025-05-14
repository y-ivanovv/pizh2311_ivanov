#include "config.h"
#include <string>

Config parseArgs(int argc, char** argv) {
    Config config = { 0, 0, "", "", 0, 0 };
    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];
        if (arg == "-l" || arg == "--length") {
            config.length = static_cast<uint16_t>(std::stoi(argv[++i]));
        }
        else if (arg == "-w" || arg == "--width") {
            config.width = static_cast<uint16_t>(std::stoi(argv[++i]));
        }
        else if (arg == "-i" || arg == "--input") {
            config.input_file = argv[++i];
        }
        else if (arg == "-o" || arg == "--output") {
            config.output_dir = argv[++i];
        }
        else if (arg == "-m" || arg == "--max-iter") {
            config.max_iter = std::stoull(argv[++i]);
        }
        else if (arg == "-f" || arg == "--freq") {
            config.freq = std::stoull(argv[++i]);
        }
    }
    return config;
}