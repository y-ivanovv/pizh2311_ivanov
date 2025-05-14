#include "parser.h"
#include <algorithm>
#include <cctype>
#include <fstream>
#include <regex>
#include <sstream>

namespace omfl {

    Node::Node() {
        _type = Type::SECTION;
        _sec = std::make_shared<Section>();
    }

    Node::Node(std::shared_ptr<Section> sec) {
        _type = Type::SECTION;
        _sec = std::move(sec);
    }

    Node::Node(int v) {
        _type = Type::INT;
        _i = v;
    }

    Node::Node(double v) {
        _type = Type::FLOAT;
        _f = v;
    }

    Node::Node(bool v) {
        _type = Type::BOOL;
        _b = v;
    }

    Node::Node(const std::string& v) {
        _type = Type::STRING;
        _s = v;
    }

    Node::Node(const std::vector<Node>& v) {
        _type = Type::ARRAY;
        _a = v;
    }

    bool Node::valid() const {
        if (_type == Type::SECTION) return _sec->valid;
        return true;
    }

    Node Node::Get(const std::string& key) const {
        if (_type != Type::SECTION) return Node{};
        return _sec->GetNode(key);
    }

    bool Node::IsInt()    const { return _type == Type::INT; }
    bool Node::IsFloat()  const { return _type == Type::FLOAT; }
    bool Node::IsBool()   const { return _type == Type::BOOL; }
    bool Node::IsString() const { return _type == Type::STRING; }
    bool Node::IsArray()  const { return _type == Type::ARRAY; }

    int Node::AsInt() const { return _i; }
    int Node::AsIntOrDefault(int d) const { return _type == Type::INT ? _i : d; }
    double Node::AsFloat() const { return _f; }
    double Node::AsFloatOrDefault(double d) const { return _type == Type::FLOAT ? _f : d; }
    bool Node::AsBool() const { return _b; }
    bool Node::AsBoolOrDefault(bool d) const { return _type == Type::BOOL ? _b : d; }
    std::string Node::AsString() const { return _s; }
    std::string Node::AsStringOrDefault(const std::string& d) const {
        return _type == Type::STRING ? _s : d;
    }

    const Node& Node::operator[](size_t idx) const {
        static Node empty;
        if (_type == Type::ARRAY && idx < _a.size()) {
            return _a[idx];
        }
        return empty;
    }

    static inline std::string trim(const std::string& s) {
        auto b = s.find_first_not_of(" \t\r\n");
        if (b == std::string::npos) return "";
        auto e = s.find_last_not_of(" \t\r\n");
        return s.substr(b, e - b + 1);
    }

    static bool isValidKey(const std::string& key) {
        static const std::regex re("^[A-Za-z0-9_-]+$");
        return std::regex_match(key, re);
    }

    static void splitOnce(const std::string& s, std::string& first, std::string& rest) {
        auto p = s.find('.');
        if (p == std::string::npos) {
            first = s;
            rest = "";
        }
        else {
            first = s.substr(0, p);
            rest = s.substr(p + 1);
        }
    }

    Node Section::GetNode(const std::string& key) const {
        std::string first, rest;
        splitOnce(key, first, rest);

        auto it_v = values.find(first);
        if (it_v != values.end() && rest.empty()) {
            return it_v->second;
        }

        auto it_s = sections.find(first);
        if (it_s != sections.end()) {
            if (rest.empty()) {
                return Node(it_s->second);
            }
            else {
                return it_s->second->GetNode(rest);
            }
        }

        return Node{};
    }

    static Node parseValue(const std::string& tok, bool& ok);

    static Node parseArray(const std::string& s, bool& ok) {
        ok = false;
        std::vector<Node> elems;
        size_t i = 1;

        while (i < s.size()) {
            while (i < s.size() && isspace((unsigned char)s[i])) ++i;
            if (i < s.size() && s[i] == ']') {
                ok = true;
                return Node(elems);
            }

            int depth = 0;
            bool in_str = false;
            size_t start = i;
            while (i < s.size()) {
                char c = s[i];
                if (c == '"' && (i == 0 || s[i - 1] != '\\')) in_str = !in_str;
                if (!in_str) {
                    if (c == '[') depth++;
                    if (c == ']') {
                        if (depth == 0) break;
                        depth--;
                    }
                    if (c == ',' && depth == 0) break;
                }
                ++i;
            }

            std::string part = trim(s.substr(start, i - start));
            bool lok;
            Node v = parseValue(part, lok);
            if (!lok) return Node{};
            elems.push_back(v);

            if (i < s.size() && s[i] == ',') ++i;
        }

        return Node{};
    }

    static Node parseValue(const std::string& tok, bool& ok) {
        std::string s = trim(tok);
        ok = false;
        if (s.empty()) return Node{};

        if (s.front() == '"' && s.back() == '"') {
            auto next_quote = s.find('"', 1);
            if (next_quote != s.size() - 1) {
                ok = false;
                return Node{};
            }
            ok = true;
            return Node(s.substr(1, s.size() - 2));
        }

        if (s.front() == '[' && s.back() == ']') {
            return parseArray(s, ok);
        }

        if (s == "true") { ok = true; return Node(true); }
        if (s == "false") { ok = true; return Node(false); }


        static const std::regex intRe("^[+-]?[0-9]+$");
        static const std::regex floatRe("^[+-]?[0-9]+\\.[0-9]+$");
        if (std::regex_match(s, intRe)) {
            ok = true;
            return Node(std::stoi(s));
        }
        if (std::regex_match(s, floatRe)) {
            ok = true;
            return Node(std::stod(s));
        }

        return Node{};
    }

    Node parse(const std::filesystem::path& path) {
        std::ifstream in(path);
        std::ostringstream oss;
        oss << in.rdbuf();
        return parse(oss.str());
    }

    Node parse(const std::string& str) {
        auto root = std::make_shared<Section>();
        auto current = root;

        std::istringstream in(str);
        std::string raw;
        while (std::getline(in, raw)) {
            auto line = trim(raw.substr(0, raw.find('#')));
            if (line.empty()) continue;

            if (line.front() == '[' && line.back() == ']') {
                std::string name = trim(line.substr(1, line.size() - 2));

                if (name.empty() || name.front() == '.' || name.back() == '.') {
                    root->valid = false;
                    break;
                }
                current = root;
                std::istringstream ss(name);
                std::string part;
                while (std::getline(ss, part, '.')) {
                    if (!isValidKey(part)) {
                        root->valid = false;
                        break;
                    }
                    auto it = current->sections.find(part);
                    if (it == current->sections.end()) {
                        auto child = std::make_shared<Section>();
                        current->sections[part] = child;
                        current = child;
                    }
                    else {
                        current = it->second;
                    }
                }
                continue;
            }

            auto eq = line.find('=');
            if (eq == std::string::npos) {
                root->valid = false;
                break;
            }
            std::string key = trim(line.substr(0, eq));
            std::string val = trim(line.substr(eq + 1));
            if (key.empty() || !isValidKey(key)) {
                root->valid = false;
                break;
            }
            if (current->values.count(key) || current->sections.count(key)) {
                root->valid = false;
                break;
            }
            bool ok;
            Node v = parseValue(val, ok);
            if (!ok) {
                root->valid = false;
                break;
            }
            current->values.emplace(key, std::move(v));
        }

        return Node(root);
    }

}
