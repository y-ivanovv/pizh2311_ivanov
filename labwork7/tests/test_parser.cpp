#include <gtest/gtest.h>
#include <fstream>
#include "lib//AudioParser.h"

class TempAudioFile {
public:
    std::string filename;

    TempAudioFile(const std::string& content) : filename("temp_audio.mp3") {
        std::ofstream file(filename, std::ios::binary);
        file.write(content.data(), content.size());
    }

    ~TempAudioFile() {
        std::remove(filename.c_str());
    }
};

TEST(TagSizeTest, BasicCalculation) {
    std::array<uint8_t, 4> bytes = { 0x00, 0x00, 0x00, 0x7F };
    EXPECT_EQ(computeTagSize(bytes), 127u);
}

TEST(TagSizeTest, MaximumSize) {
    std::array<uint8_t, 4> bytes = { 0x7F, 0x7F, 0x7F, 0x7F };
    EXPECT_EQ(computeTagSize(bytes), 0x0FFFFFFFu);
}

TEST(FrameTest, BaseFrameOutput) {
    std::vector<uint8_t> data = { 0x01, 0x02 };
    ID3Frame frame("TEST", 2, 0, data);

    testing::internal::CaptureStdout();
    frame.printInfo();
    std::string output = testing::internal::GetCapturedStdout();

    EXPECT_EQ(output, "TEST [2 bytes]");
}

TEST(TextFrameTest, SimpleText) {
    std::vector<uint8_t> data = { 0x00, 'H', 'i' };
    TextFrame frame("TIT2", data.size(), 0, data);

    testing::internal::CaptureStdout();
    frame.printInfo();
    std::string output = testing::internal::GetCapturedStdout();

    EXPECT_EQ(output, "Text frame TIT2 (Title): Hi");
}

TEST(URLFrameTest, SimpleURL) {
    std::vector<uint8_t> data = { 'h','t','t','p',':','/','/','t','e','s','t' };
    URLFrame frame("WXXX", data.size(), 0, data);

    testing::internal::CaptureStdout();
    frame.printInfo();
    std::string output = testing::internal::GetCapturedStdout();

    EXPECT_EQ(output, "URL frame WXXX: http://test");
}

TEST(CommentFrameTest, ValidComment) {
    std::vector<uint8_t> data = { 0x00, 'e','n','g', 0x00, 'T','e','s','t' };
    CommentFrame frame("COMM", data.size(), 0, data);

    testing::internal::CaptureStdout();
    frame.printInfo();
    std::string output = testing::internal::GetCapturedStdout();

    EXPECT_EQ(output, "Comment frame COMM [eng]: Test");
}

TEST(FactoryTest, CreateTextFrame) {
    std::vector<uint8_t> data = { 0x00 };
    auto frame = createFrame("TALB", 1, 0, data);
    EXPECT_NE(dynamic_cast<TextFrame*>(frame.get()), nullptr);
}

TEST(DescriptionTest, KnownDescriptions) {
    EXPECT_EQ(getFrameDescription("TIT2"), "Title");
    EXPECT_EQ(getFrameDescription("TALB"), "Album");
    EXPECT_EQ(getFrameDescription("COMM"), "Comments");
}