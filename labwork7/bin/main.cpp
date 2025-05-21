#include <fstream>
#include <iostream>
#include "lib//AudioParser.h"

constexpr int ID3_HEADER_SIZE = 10;

void showErrorMessage(const std::string& text) {
    std::cerr << "Error: " << text << '\n';
}

bool isHeaderValid(const std::string& header) {
    return header.size() == ID3_HEADER_SIZE && header.substr(0, 3) == "ID3";
}

struct ID3Tag {
    uint8_t majorVersion;
    uint8_t minorVersion;
    uint8_t flags;
    uint32_t tagSize;
};

ID3Tag parseID3Header(const std::string& header) {
    return {
        static_cast<uint8_t>(header[3]),
        static_cast<uint8_t>(header[4]),
        static_cast<uint8_t>(header[5]),
        computeTagSize({
            static_cast<uint8_t>(header[6]),
            static_cast<uint8_t>(header[7]),
            static_cast<uint8_t>(header[8]),
            static_cast<uint8_t>(header[9])
        })
    };
}

int analyzeAudioFile(const char* filename) {
    std::ifstream file(filename, std::ios::binary);
    if (!file) {
        showErrorMessage("Cannot open file");
        return 2;
    }

    std::string header(ID3_HEADER_SIZE, '\0');
    file.read(&header[0], ID3_HEADER_SIZE);

    if (!isHeaderValid(header)) {
        showErrorMessage("Invalid ID3 tag");
        return 3;
    }

    auto tag = parseID3Header(header);
    std::cout << "Found ID3v2." << (int)tag.majorVersion << "." << (int)tag.minorVersion
        << ", total size: " << tag.tagSize << " bytes\n";

    processID3Frames(file, tag.tagSize);
    return 0;
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        showErrorMessage("Please provide audio file path");
        return 1;
    }
    return analyzeAudioFile(argv[1]);
}