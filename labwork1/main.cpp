#include <cctype>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

void showUsage(std::string name) {
    std::cerr << "Usage: " << name << " [OPTION] filename [filename,...]*\n"
        << "Options:\n"
        << "\t-l, --lines\tOutput only the number of lines\n"
        << "\t-c, --bytes\tOutput of file size in bytes\n"
        << "\t-w, --words\tOutput of the number of words\n"
        << "\t-m, --chars\tOutput of the number of letters\n"
        << std::endl;
}

void countLines(const std::vector<std::string>& filenames) {
    for (const auto& filename : filenames) {
        std::ifstream file(filename);

        if (!file.is_open()) {
            std::cerr << "Error opening file: " << filename << std::endl;
            continue;
        }

        int lineCount = 0;
        std::string line;

        while (std::getline(file, line)) {
            lineCount++;
        }

        std::cout << "The number of lines: " << lineCount << " " << filename << std::endl;
    }
}

void getSize(const std::vector<std::string>& filenames) {
    for (const auto& filename : filenames) {
        std::ifstream file(filename, std::ios::binary | std::ios::ate);

        if (!file.is_open()) {
            std::cerr << "Error opening file: " << filename << std::endl;
            continue;
        }

        std::streamsize size = file.tellg();

        std::cout << "File size in bytes: " << size << " " << filename << std::endl;
    }
}

void countWords(const std::vector<std::string>& filenames) {
    for (const auto& filename : filenames) {
        std::ifstream file(filename);

        if (!file.is_open()) {
            std::cerr << "Error opening file: " << filename << std::endl;
            continue;
        }

        int wordCount = 0;
        std::string word;

        while (file >> word) {
            wordCount++;
        }

        std::cout << "The number of words: " << wordCount << " " << filename << std::endl;
    }
}

void countChars(const std::vector<std::string>& filenames) {
    for (const auto& filename : filenames) {
        std::ifstream file(filename);

        if (!file.is_open()) {
            std::cerr << "Error opening file: " << filename << std::endl;
            continue;
        }

        int charCount = 0;
        char ch;

        while (file.get(ch)) {
            if (std::isalpha(ch)) {
                charCount++;
            }
        }

        std::cout << "The number of letters: " << charCount << " " << filename << std::endl;
    }
}

int main(int argc, char** argv) {
    std::vector<std::string> filenames;
    std::vector<std::string> commands;

    if (argc < 2) {
        showUsage(argv[0]);
        return 1;
    }

    for (int i = 1; i < argc; i++) {
        std::string arg = argv[i];
        if (arg[0] == '-') {
            if ((arg == "-l") || (arg == "--lines")) {
                commands.push_back("lines");
            }
            else if ((arg == "-c") || (arg == "--bytes")) {
                commands.push_back("bytes");
            }
            else if ((arg == "-w") || (arg == "--words")) {
                commands.push_back("words");
            }
            else if ((arg == "-m") || (arg == "--chars")) {
                commands.push_back("chars");
            }
            else {
                std::cerr << "Unknown option: " << arg << std::endl;
                showUsage(argv[0]);
                return 1;
            }
        }
        else {
            filenames.push_back(arg);
        }
    }

    if (filenames.empty()) {
        std::cerr << "No filenames provided" << std::endl;
        showUsage(argv[0]);
        return 1;
    }

    if (commands.empty()) {
        countLines(filenames);
        getSize(filenames);
        countWords(filenames);
        countChars(filenames);
    }
    else {
        for (const auto& cmd : commands) {
            if (cmd == "lines") {
                countLines(filenames);
            }
            else if (cmd == "bytes") {
                getSize(filenames);
            }
            else if (cmd == "words") {
                countWords(filenames);
            }
            else if (cmd == "chars") {
                countChars(filenames);
            }
        }
    }

    return 0;
}
