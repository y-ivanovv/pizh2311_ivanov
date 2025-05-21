#include <gtest/gtest.h>
#include <vector>
#include <map>
#include <list>
#include "adapters.h"

using namespace collection_adapters;

TEST(FilterTest, OddNumbers) {
    std::vector<int> numbers = { 1, 2, 3, 4, 5 };
    std::vector<int> expected = { 1, 3, 5 };
    std::vector<int> result;

    for (int num : numbers | where([](int x) { return x % 2; })) {
        result.push_back(num);
    }

    ASSERT_EQ(result, expected);
}

TEST(MapTest, SquareNumbers) {
    std::vector<int> numbers = { 1, 2, 3 };
    std::vector<int> expected = { 1, 4, 9 };
    std::vector<int> result;

    for (int num : numbers | map([](int x) { return x * x; })) {
        result.push_back(num);
    }

    ASSERT_EQ(result, expected);
}

TEST(LimitTest, FirstThree) {
    std::vector<int> numbers = { 10, 20, 30, 40, 50 };
    std::vector<int> expected = { 10, 20, 30 };
    std::vector<int> result;

    for (int num : numbers | limit(3)) {
        result.push_back(num);
    }

    ASSERT_EQ(result, expected);
}

TEST(SkipTest, AfterFirstTwo) {
    std::vector<int> numbers = { 5, 6, 7, 8, 9 };
    std::vector<int> expected = { 7, 8, 9 };
    std::vector<int> result;

    for (int num : numbers | skip(2)) {
        result.push_back(num);
    }

    ASSERT_EQ(result, expected);
}

TEST(ReverseTest, Characters) {
    std::vector<char> chars = { 'a', 'b', 'c', 'd' };
    std::vector<char> expected = { 'd', 'c', 'b', 'a' };
    std::vector<char> result;

    for (char c : reversed(chars)) {
        result.push_back(c);
    }

    ASSERT_EQ(result, expected);
}

TEST(KeysTest, MapKeys) {
    std::map<std::string, int> dictionary = { {"one", 1}, {"two", 2}, {"three", 3} };
    std::vector<std::string> expected = { "one", "three", "two" };
    std::vector<std::string> result;

    for (const auto& key : get_keys(dictionary)) {
        result.push_back(key);
    }

    ASSERT_EQ(result, expected);
}

TEST(ValuesTest, MapValues) {
    std::map<std::string, int> dictionary = { {"a", 10}, {"b", 20}, {"c", 30} };
    std::vector<int> expected = { 10, 20, 30 };
    std::vector<int> result;

    for (int value : get_values(dictionary)) {
        result.push_back(value);
    }

    ASSERT_EQ(result, expected);
}

TEST(PipelineTest, CombinedOperations) {
    std::list<int> sequence = { 1, 2, 3, 4, 5, 6 };
    std::vector<int> expected = { 1, 9 };
    std::vector<int> result;

    for (int num : sequence |
        where([](int x) { return x % 2; }) |
        map([](int x) { return x * x; }) |
        limit(2)) {
        result.push_back(num);
    }

    ASSERT_EQ(result, expected);
}