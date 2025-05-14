#include "lib/parser.h"
#include <iostream>
#include <fstream>
#include <sstream>

int main(int argc, char** argv) {
    if (argc < 2) {
        std::cerr << "Usage: " << argv[0] << " <file.omfl>" << std::endl;

        return 1;
    }

    std::ifstream file(argv[1]);
    if (!file) {
        std::cerr << "Error: File not found or cannot be opened." << std::endl;

        return 1;
    }

    std::stringstream buffer;
    buffer << file.rdbuf();
    std::string file_content = buffer.str();
    std::cout << "File content:\n" << file_content << "\n";

    auto root = omfl::parse(file_content);
    if (!root.valid()) {
        std::cerr << "Error: Invalid OMFL file. Check syntax (e.g., unclosed quotes, brackets, or invalid keys)." << std::endl;

        return 1;
    }

    std::cout << "File parsed successfully!\n";
    std::cout << "Common name: " << root.Get("common.name").AsString() << std::endl;

    return 0;
}
