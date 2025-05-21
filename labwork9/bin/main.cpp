#include <iostream>
#include <vector>
#include <list>
#include <map>
#include "adapters.h"

int main() {
    using namespace collection_adapters;

    std::vector<int> numbers = { 1,2,3,4,5,6 };

    std::cout << "Squares of odd numbers: ";
    for (int num : numbers | where([](int n) { return n % 2; })
        | map([](int n) { return n * n; })) {
        std::cout << num << " ";
    }
    std::cout << std::endl;

    std::cout << "First 3 elements: ";
    for (int num : numbers | limit(3)) std::cout << num << " ";
    std::cout << std::endl;

    std::cout << "After skipping 2 elements: ";
    for (int num : numbers | skip(2)) std::cout << num << " ";
    std::cout << std::endl;

    std::cout << "Reversed numbers: ";
    for (int num : reversed(numbers)) std::cout << num << " ";
    std::cout << std::endl;

    std::map<std::string, int> dictionary = { {"a",10}, {"b",20}, {"c",30} };
    std::cout << "Dictionary keys: ";
    for (const auto& key : get_keys(dictionary)) std::cout << key << " ";
    std::cout << std::endl;

    std::cout << "Dictionary values: ";
    for (auto value : get_values(dictionary)) std::cout << value << " ";
    std::cout << std::endl;

    std::list<int> sequence = { 1,2,3,4,5,6 };
    std::cout << "Complex pipeline result: ";
    for (int num : sequence
        | where([](int n) { return n % 2; })
        | map([](int n) { return n * n; })
        | limit(2)) {
        std::cout << num << " ";
    }
    std::cout << std::endl;

    return 0;
}