#pragma once
#include <memory>
#include <string>
#include <vector>
#include <array>
#include <unordered_map>

uint32_t computeTagSize(const std::array<uint8_t, 4>& bytes);

class ID3Frame {
public:
    std::string frameID;
    uint32_t size;
    uint16_t flags;
    std::vector<uint8_t> data;

    ID3Frame(const std::string& id, uint32_t sz, uint16_t fl, const std::vector<uint8_t>& dt);
    virtual void printInfo() const;
    virtual ~ID3Frame() = default;
};

class TextFrame : public ID3Frame {
public:
    TextFrame(const std::string& id, uint32_t sz, uint16_t fl, const std::vector<uint8_t>& dt);
    void printInfo() const override;
};

class URLFrame : public ID3Frame {
public:
    URLFrame(const std::string& id, uint32_t sz, uint16_t fl, const std::vector<uint8_t>& dt);
    void printInfo() const override;
};

class CommentFrame : public ID3Frame {
public:
    CommentFrame(const std::string& id, uint32_t sz, uint16_t fl, const std::vector<uint8_t>& dt);
    void printInfo() const override;
};

class GenericFrame : public ID3Frame {
public:
    using ID3Frame::ID3Frame;
};

std::string getFrameDescription(const std::string& id);
std::unique_ptr<ID3Frame> createFrame(const std::string& id, uint32_t size, uint16_t flags, const std::vector<uint8_t>& data);
void processID3Frames(std::ifstream& file, uint32_t totalSize);