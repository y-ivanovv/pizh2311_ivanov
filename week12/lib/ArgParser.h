#pragma once

#include <string>
#include <vector>
#include <map>
#include <stdexcept>
#include <algorithm>
#include <sstream>
#include <iostream>

namespace ArgumentParser {

class ArgParser {
public:
    explicit ArgParser(const std::string& name);

    class StringArgument;
    StringArgument& AddStringArgument(const std::string& long_name);
    StringArgument& AddStringArgument(char short_name, const std::string& long_name);
    StringArgument& AddStringArgument(const std::string& long_name, const std::string& description);
    std::string GetStringValue(const std::string& name, size_t index = 0) const;

    class IntArgument;
    IntArgument& AddIntArgument(const std::string& long_name);
    IntArgument& AddIntArgument(char short_name, const std::string& long_name);
    IntArgument& AddIntArgument(const std::string& long_name, const std::string& description);
    int GetIntValue(const std::string& name, size_t index = 0) const;

    class Flag;
    Flag& AddFlag(const std::string& long_name);
    Flag& AddFlag(char short_name, const std::string& long_name);
    Flag& AddFlag(const std::string& long_name, const std::string& description);
    bool GetFlag(const std::string& name) const;

    void AddHelp(char short_name, const std::string& long_name, const std::string& description);
    bool Help() const;
    std::string HelpDescription() const;

    bool Parse(int argc, char** argv);
    bool Parse(const std::vector<std::string>& args);

private:
    std::string program_name_;
    std::string help_description_;
    char help_short_name_ = '\0';
    std::string help_long_name_;
    bool help_defined_ = false;
    bool help_requested_ = false;

    std::vector<std::string> positional_args_names_;
    std::map<std::string, StringArgument*> string_args_;
    std::map<std::string, IntArgument*> int_args_;
    std::map<std::string, Flag*> flag_args_;
    std::map<char, std::string> short_to_long_names_;

    std::vector<std::string> tokenize(int argc, char** argv);
    bool validateArguments();
    std::string getLongName(char short_name) const;
    bool parseArgument(const std::string& arg, size_t& i, const std::vector<std::string>& args);
    bool parsePositionalArguments(size_t& i, const std::vector<std::string>& args);
};

class ArgParser::StringArgument {
public:
    StringArgument& Default(const std::string& value);
    StringArgument& StoreValue(std::string& value);
    StringArgument& MultiValue(size_t min_args = 0);
    StringArgument& Positional();
    
    bool IsPositional() const;
    const std::string& GetName() const;
    const std::string& GetDescription() const;
    bool IsMultiValue() const;
    size_t GetMinArgs() const;
    bool HasDefault() const;
    const std::string& GetDefault() const;
    const std::vector<std::string>& GetValues() const;
    bool IsStored() const;
    std::string& GetStoredValue();
    std::vector<std::string>& GetStoredValues();

private:
    friend class ArgParser;

    std::string long_name_;
    char short_name_ = '\0';
    std::string description_;
    bool is_positional_ = false;
    bool is_multi_value_ = false;
    size_t min_args_ = 0;
    bool has_default_ = false;
    std::string default_value_;
    std::vector<std::string> values_;
    std::string* stored_value_ = nullptr;
    std::vector<std::string>* stored_values_ = nullptr;
};

class ArgParser::IntArgument {
public:
    IntArgument& Default(int value);
    IntArgument& StoreValue(int& value);
    IntArgument& StoreValues(std::vector<int>& values);
    IntArgument& MultiValue(size_t min_args = 0);
    IntArgument& Positional();
    
    bool IsPositional() const;
    const std::string& GetName() const;
    const std::string& GetDescription() const;
    bool IsMultiValue() const;
    size_t GetMinArgs() const;
    bool HasDefault() const;
    int GetDefault() const;
    const std::vector<int>& GetValues() const;
    bool IsStored() const;
    int& GetStoredValue();
    std::vector<int>& GetStoredValues();

private:
    friend class ArgParser;

    std::string long_name_;
    char short_name_ = '\0';
    std::string description_;
    bool is_positional_ = false;
    bool is_multi_value_ = false;
    size_t min_args_ = 0;
    bool has_default_ = false;
    int default_value_ = 0;
    std::vector<int> values_;
    int* stored_value_ = nullptr;
    std::vector<int>* stored_values_ = nullptr;
};

class ArgParser::Flag {
public:
    Flag& Default(bool value);
    Flag& StoreValue(bool& value);
    
    const std::string& GetName() const;
    const std::string& GetDescription() const;
    bool HasDefault() const;
    bool GetDefault() const;
    bool GetValue() const;
    bool IsStored() const;
    bool& GetStoredValue();

private:
    friend class ArgParser;

    std::string long_name_;
    char short_name_ = '\0';
    std::string description_;
    bool has_default_ = false;
    bool default_value_ = false;
    bool value_ = false;
    bool* stored_value_ = nullptr;
};

}