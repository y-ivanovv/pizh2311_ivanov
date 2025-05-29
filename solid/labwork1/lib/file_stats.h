#ifndef FILE_STATS_H
#define FILE_STATS_H

#include <string>
#include <vector>
#include <memory>
#include <map>
#include <functional>

class HelpDisplayer {
public:
    static void showUsage(const std::string& name);
};

class FileOperation {
public:
    virtual ~FileOperation() = default;
    virtual void execute(const std::string& filename) const = 0;
    virtual std::string getName() const = 0;
};

class LineCountOperation : public FileOperation {
public:
    void execute(const std::string& filename) const override;
    std::string getName() const override { return "lines"; }
};

class ByteSizeOperation : public FileOperation {
public:
    void execute(const std::string& filename) const override;
    std::string getName() const override { return "bytes"; }
};

class WordCountOperation : public FileOperation {
public:
    void execute(const std::string& filename) const override;
    std::string getName() const override { return "words"; }
};

class CharCountOperation : public FileOperation {
public:
    void execute(const std::string& filename) const override;
    std::string getName() const override { return "chars"; }
};

class OperationFactory {
public:
    static std::unique_ptr<FileOperation> create(const std::string& operationName);
};

class CommandProcessor {
public:
    static std::vector<std::string> processCommands(int argc, char** argv, std::vector<std::string>& filenames);
};

class FileStatsApplication {
public:
    void run(int argc, char** argv);
};

#endif