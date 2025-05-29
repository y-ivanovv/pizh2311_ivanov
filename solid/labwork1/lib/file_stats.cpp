#include "file_stats.h"
#include <fstream>
#include <iostream>
#include <cctype>
#include <stdexcept>

void HelpDisplayer::showUsage(const std::string& name) {
    std::cerr << "Usage: " << name << " [OPTION] filename [filename,...]*\n"
        << "Options:\n"
        << "\t-l, --lines\tOutput only the number of lines\n"
        << "\t-c, --bytes\tOutput of file size in bytes\n"
        << "\t-w, --words\tOutput of the number of words\n"
        << "\t-m, --chars\tOutput of the number of letters\n"
        << std::endl;
}

void LineCountOperation::execute(const std::string& filename) const {
    std::ifstream file(filename);
    if (!file.is_open()) {
        throw std::runtime_error("Error opening file: " + filename);
    }

    int lineCount = 0;
    std::string line;
    while (std::getline(file, line)) {
        lineCount++;
    }

    std::cout << "The number of lines: " << lineCount << " " << filename << std::endl;
}

void ByteSizeOperation::execute(const std::string& filename) const {
    std::ifstream file(filename, std::ios::binary | std::ios::ate);
    if (!file.is_open()) {
        throw std::runtime_error("Error opening file: " + filename);
    }

    std::streamsize size = file.tellg();
    std::cout << "File size in bytes: " << size << " " << filename << std::endl;
}

void WordCountOperation::execute(const std::string& filename) const {
    std::ifstream file(filename);
    if (!file.is_open()) {
        throw std::runtime_error("Error opening file: " + filename);
    }

    int wordCount = 0;
    std::string word;
    while (file >> word) {
        wordCount++;
    }

    std::cout << "The number of words: " << wordCount << " " << filename << std::endl;
}

void CharCountOperation::execute(const std::string& filename) const {
    std::ifstream file(filename);
    if (!file.is_open()) {
        throw std::runtime_error("Error opening file: " + filename);
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

std::unique_ptr<FileOperation> OperationFactory::create(const std::string& operationName) {
    static const std::map<std::string, std::function<std::unique_ptr<FileOperation>()>> operations = {
        {"lines", []() { return std::make_unique<LineCountOperation>(); }},
        {"bytes", []() { return std::make_unique<ByteSizeOperation>(); }},
        {"words", []() { return std::make_unique<WordCountOperation>(); }},
        {"chars", []() { return std::make_unique<CharCountOperation>(); }}
    };

    auto it = operations.find(operationName);
    if (it != operations.end()) {
        return it->second();
    }
    return nullptr;
}

std::vector<std::string> CommandProcessor::processCommands(int argc, char** argv, std::vector<std::string>& filenames) {
    std::vector<std::string> commands;

    for (int i = 1; i < argc; i++) {
        std::string arg = argv[i];
        if (arg[0] == '-') {
            static const std::map<std::string, std::string> optionMap = {
                {"-l", "lines"}, {"--lines", "lines"},
                {"-c", "bytes"}, {"--bytes", "bytes"},
                {"-w", "words"}, {"--words", "words"},
                {"-m", "chars"}, {"--chars", "chars"}
            };

            auto it = optionMap.find(arg);
            if (it != optionMap.end()) {
                commands.push_back(it->second);
            }
            else {
                throw std::invalid_argument("Unknown option: " + arg);
            }
        }
        else {
            filenames.push_back(arg);
        }
    }

    return commands;
}

void FileStatsApplication::run(int argc, char** argv) {
    try {
        std::vector<std::string> filenames;
        auto commands = CommandProcessor::processCommands(argc, argv, filenames);

        if (filenames.empty()) {
            throw std::invalid_argument("No filenames provided");
        }

        std::vector<std::unique_ptr<FileOperation>> operations;

        if (commands.empty()) {
            operations.push_back(std::make_unique<LineCountOperation>());
            operations.push_back(std::make_unique<ByteSizeOperation>());
            operations.push_back(std::make_unique<WordCountOperation>());
            operations.push_back(std::make_unique<CharCountOperation>());
        }
        else {
            for (const auto& cmd : commands) {
                auto op = OperationFactory::create(cmd);
                if (op) {
                    operations.push_back(std::move(op));
                }
            }
        }

        for (const auto& filename : filenames) {
            for (const auto& op : operations) {
                try {
                    op->execute(filename);
                }
                catch (const std::exception& e) {
                    std::cerr << e.what() << std::endl;
                }
            }
        }
    }
    catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        HelpDisplayer::showUsage(argv[0]);
    }
}