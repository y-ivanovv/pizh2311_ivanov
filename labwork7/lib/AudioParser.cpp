#include "AudioParser.h"
#include <algorithm>
#include <fstream>
#include <iostream>

uint32_t computeTagSize(const std::array<uint8_t, 4>& bytes) {
    return (bytes[0] << 21) | (bytes[1] << 14) | (bytes[2] << 7) | bytes[3];
}

ID3Frame::ID3Frame(const std::string& id, uint32_t sz, uint16_t fl, const std::vector<uint8_t>& dt)
    : frameID(id), size(sz), flags(fl), data(dt) {}

void ID3Frame::printInfo() const {
    std::cout << frameID << " [" << size << " bytes]";
}

TextFrame::TextFrame(const std::string& id, uint32_t sz, uint16_t fl, const std::vector<uint8_t>& dt)
    : ID3Frame(id, sz, fl, dt) {}

void TextFrame::printInfo() const {
    std::string text;
    if (!data.empty()) {
        text.assign(data.begin() + 1, data.end());
    }
    std::cout << "Text frame " << frameID;
    auto desc = getFrameDescription(frameID);
    if (!desc.empty()) std::cout << " (" << desc << ")";
    std::cout << ": " << text;
}

URLFrame::URLFrame(const std::string& id, uint32_t sz, uint16_t fl, const std::vector<uint8_t>& dt)
    : ID3Frame(id, sz, fl, dt) {}

void URLFrame::printInfo() const {
    std::string url(data.begin(), data.end());
    std::cout << "URL frame " << frameID << ": " << url;
}

CommentFrame::CommentFrame(const std::string& id, uint32_t sz, uint16_t fl, const std::vector<uint8_t>& dt)
    : ID3Frame(id, sz, fl, dt) {}

void CommentFrame::printInfo() const {
    if (data.size() < 4) {
        std::cout << "Invalid comment frame";
        return;
    }
    std::string language(data.begin() + 1, data.begin() + 4);
    auto textStart = std::find(data.begin() + 4, data.end(), 0);
    std::string comment(textStart + 1, data.end());
    std::cout << "Comment frame " << frameID << " [" << language << "]: " << comment;
}

std::string getFrameDescription(const std::string& id) {
    static const std::unordered_map<std::string, std::string> descriptions = {
        {"TIT2", "Title"}, {"TPE1", "Artist"}, {"TALB", "Album"},
        {"TRCK", "Track number"}, {"TYER", "Year"}, {"TDRC", "Recording date"},
        {"TCON", "Genre"}, {"COMM", "Comments"}, {"TENC", "Encoder"},
        {"TCOM", "Composer"}, {"TBPM", "BPM"}, {"TSSE", "Encoder settings"},
        {"TPOS", "Part of set"}
    };
    auto it = descriptions.find(id);
    return it != descriptions.end() ? it->second : "";
}

std::unique_ptr<ID3Frame> createFrame(const std::string& id, uint32_t size, uint16_t flags, const std::vector<uint8_t>& data) {
    if (id.empty()) return std::make_unique<GenericFrame>(id, size, flags, data);

    switch (id[0]) {
    case 'T': return std::make_unique<TextFrame>(id, size, flags, data);
    case 'W': return std::make_unique<URLFrame>(id, size, flags, data);
    case 'C': return id == "COMM" ? std::make_unique<CommentFrame>(id, size, flags, data) : nullptr;
    default: return std::make_unique<GenericFrame>(id, size, flags, data);
    }
}

void processID3Frames(std::ifstream& file, uint32_t totalSize) {
    uint32_t processedBytes = 0;

    while (processedBytes + 10 <= totalSize) {
        std::string frameID(4, '\0');
        file.read(&frameID[0], 4);
        if (file.gcount() != 4 || frameID == "\0\0\0\0") break;

        std::array<uint8_t, 4> sizeBytes{};
        file.read(reinterpret_cast<char*>(sizeBytes.data()), 4);
        uint32_t frameSize = computeTagSize(sizeBytes);

        uint16_t frameFlags = 0;
        file.read(reinterpret_cast<char*>(&frameFlags), 2);

        if (frameSize == 0) break;

        std::vector<uint8_t> frameData(frameSize);
        file.read(reinterpret_cast<char*>(frameData.data()), frameSize);
        if (file.gcount() != static_cast<int>(frameSize)) break;

        processedBytes += 10 + frameSize;
        auto frame = createFrame(frameID, frameSize, frameFlags, frameData);
        if (frame) {
            frame->printInfo();
            std::cout << '\n';
        }
    }
}