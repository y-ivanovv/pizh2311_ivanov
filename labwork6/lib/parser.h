#pragma once

#include <filesystem>
#include <istream>
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

namespace omfl {

    struct Section;

    class Node {
    public:
        Node();
        explicit Node(std::shared_ptr<Section> sec);
        explicit Node(int v);
        explicit Node(double v);
        explicit Node(bool v);
        explicit Node(const std::string& v);
        explicit Node(const std::vector<Node>& v);

        bool valid() const;

        Node Get(const std::string& key) const;

        bool IsInt()    const;
        bool IsFloat()  const;
        bool IsBool()   const;
        bool IsString() const;
        bool IsArray()  const;

        int         AsInt()              const;
        int         AsIntOrDefault(int d) const;
        double      AsFloat()            const;
        double      AsFloatOrDefault(double d) const;
        bool        AsBool()             const;
        bool        AsBoolOrDefault(bool d) const;
        std::string AsString()           const;
        std::string AsStringOrDefault(const std::string& d) const;
        const Node& operator[](size_t idx) const;

    private:
        enum class Type { NONE, INT, FLOAT, BOOL, STRING, ARRAY, SECTION };
        Type _type = Type::NONE;

        int                     _i{};
        double                  _f{};
        bool                    _b{};
        std::string             _s;
        std::vector<Node>       _a;
        std::shared_ptr<Section> _sec;
    };

    struct Section {
        bool valid = true;
        std::unordered_map<std::string, Node>                             values;
        std::unordered_map<std::string, std::shared_ptr<Section>> sections;

        Node GetNode(const std::string& key) const;
    };

    Node parse(const std::filesystem::path& path);
    Node parse(const std::string& str);

}
