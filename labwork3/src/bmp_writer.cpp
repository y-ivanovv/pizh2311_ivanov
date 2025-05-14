#include "bmp_writer.h"
#include "grid.h"
#include <fstream>
#include <vector>

void saveBMP(const Grid& grid, const std::string& path, int iteration) {
    int width = grid.max_x - grid.min_x + 1;
    int height = grid.max_y - grid.min_y + 1;
    int row_padding = (4 - (width * 3) % 4) % 4;
    int file_size = 54 + (width * 3 + row_padding) * height;

    std::vector<uint8_t> img_data(file_size - 54, 0);

    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            uint64_t value = grid.data[y][x];
            uint8_t r, g, b;
            if (value == 0) {
                r = g = b = 255;
            }
            else if (value == 1) {
                r = 0; g = 128; b = 0;
            }
            else if (value == 2) {
                r = 128; g = 0; b = 128;
            }
            else if (value == 3) {
                r = 255; g = 255; b = 0;
            }
            else {
                r = g = b = 0;
            }
            int pos = (y * (width * 3 + row_padding)) + x * 3;
            img_data[pos] = b;
            img_data[pos + 1] = g;
            img_data[pos + 2] = r;
        }
    }

    std::ofstream file(path, std::ios::binary);
    uint8_t header[54] = {
        'B','M',
        static_cast<uint8_t>(file_size), static_cast<uint8_t>(file_size >> 8),
        static_cast<uint8_t>(file_size >> 16), static_cast<uint8_t>(file_size >> 24),
        0,0,0,0, 54,0,0,0,
        40,0,0,0,
        static_cast<uint8_t>(width), static_cast<uint8_t>(width >> 8),
        static_cast<uint8_t>(width >> 16), static_cast<uint8_t>(width >> 24),
        static_cast<uint8_t>(height), static_cast<uint8_t>(height >> 8),
        static_cast<uint8_t>(height >> 16), static_cast<uint8_t>(height >> 24),
        1,0, 24,0,
        0,0,0,0, 0,0,0,0, 0,0,0,0, 0,0,0,0
    };
    file.write(reinterpret_cast<char*>(header), 54);
    file.write(reinterpret_cast<char*>(img_data.data()), img_data.size());
}