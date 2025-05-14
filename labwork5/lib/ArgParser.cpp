#include "ArgParser.h"

namespace ArgumentParser {

ArgParser::ArgParser(const std::string& name) : program_name_(name) {}

ArgParser::StringArgument& ArgParser::AddStringArgument(const std::string& long_name) {
    auto arg = new StringArgument();
    arg->long_name_ = long_name;
    string_args_[long_name] = arg;
    return *arg;
}

ArgParser::StringArgument& ArgParser::AddStringArgument(char short_name, const std::string& long_name) {
    auto& arg = AddStringArgument(long_name);
    arg.short_name_ = short_name;
    short_to_long_names_[short_name] = long_name;
    return arg;
}

ArgParser::StringArgument& ArgParser::AddStringArgument(const std::string& long_name, const std::string& description) {
    auto& arg = AddStringArgument(long_name);
    arg.description_ = description;
    return arg;
}

std::string ArgParser::GetStringValue(const std::string& name, size_t index) const {
    if (string_args_.count(name) == 0) {
        throw std::runtime_error("Argument not found: " + name);
    }
    const auto& arg = string_args_.at(name);
    if (index >= arg->values_.size()) {
        if (arg->has_default_ && index == 0) {
            return arg->default_value_;
        }
        throw std::runtime_error("Index out of range for argument: " + name);
    }
    return arg->values_[index];
}

ArgParser::IntArgument& ArgParser::AddIntArgument(const std::string& long_name) {
    auto arg = new IntArgument();
    arg->long_name_ = long_name;
    int_args_[long_name] = arg;
    return *arg;
}

ArgParser::IntArgument& ArgParser::AddIntArgument(char short_name, const std::string& long_name) {
    auto& arg = AddIntArgument(long_name);
    arg.short_name_ = short_name;
    short_to_long_names_[short_name] = long_name;
    return arg;
}

ArgParser::IntArgument& ArgParser::AddIntArgument(const std::string& long_name, const std::string& description) {
    auto& arg = AddIntArgument(long_name);
    arg.description_ = description;
    return arg;
}

int ArgParser::GetIntValue(const std::string& name, size_t index) const {
    if (int_args_.count(name) == 0) {
        throw std::runtime_error("Argument not found: " + name);
    }
    const auto& arg = int_args_.at(name);
    if (index >= arg->values_.size()) {
        if (arg->has_default_ && index == 0) {
            return arg->default_value_;
        }
        throw std::runtime_error("Index out of range for argument: " + name);
    }
    return arg->values_[index];
}

ArgParser::Flag& ArgParser::AddFlag(const std::string& long_name) {
    auto flag = new Flag();
    flag->long_name_ = long_name;
    flag_args_[long_name] = flag;
    return *flag;
}

ArgParser::Flag& ArgParser::AddFlag(char short_name, const std::string& long_name) {
    auto& flag = AddFlag(long_name);
    flag.short_name_ = short_name;
    short_to_long_names_[short_name] = long_name;
    return flag;
}

ArgParser::Flag& ArgParser::AddFlag(const std::string& long_name, const std::string& description) {
    auto& flag = AddFlag(long_name);
    flag.description_ = description;
    return flag;
}

bool ArgParser::GetFlag(const std::string& name) const {
    if (flag_args_.count(name) == 0) {
        throw std::runtime_error("Flag not found: " + name);
    }
    const auto& flag = flag_args_.at(name);
    return flag->value_;
}

void ArgParser::AddHelp(char short_name, const std::string& long_name, const std::string& description) {
    help_short_name_ = short_name;
    help_long_name_ = long_name;
    help_description_ = description;
    help_defined_ = true;
    short_to_long_names_[short_name] = long_name;
}

bool ArgParser::Help() const {
    return help_requested_;
}

std::string ArgParser::HelpDescription() const {
    std::stringstream help;
    help << program_name_ << "\n";
    
    if (help_defined_) {
        help << help_description_ << "\n\n";
    }
    
    for (const auto& [name, flag] : flag_args_) {
        if (flag->short_name_ != '\0') {
            help << "-" << flag->short_name_ << ", ";
        } else {
            help << "    ";
        }
        help << "--" << name;
        if (flag->has_default_) {
            help << " [default = " << (flag->default_value_ ? "true" : "false") << "]";
        }
        help << ",  " << flag->description_ << "\n";
    }

    for (const auto& [name, arg] : string_args_) {
        if (arg->short_name_ != '\0') {
            help << "-" << arg->short_name_ << ", ";
        } else {
            help << "    ";
        }
        help << "--" << name << "=<string>";
        if (arg->is_multi_value_) {
            help << " [repeated, min args = " << arg->min_args_ << "]";
        }
        if (arg->has_default_) {
            help << " [default = " << arg->default_value_ << "]";
        }
        help << ",  " << arg->description_ << "\n";
    }
    
    for (const auto& [name, arg] : int_args_) {
        if (arg->short_name_ != '\0') {
            help << "-" << arg->short_name_ << ", ";
        } else {
            help << "    ";
        }
        help << "--" << name << "=<int>";
        if (arg->is_multi_value_) {
            help << " [repeated, min args = " << arg->min_args_ << "]";
        }
        if (arg->has_default_) {
            help << " [default = " << arg->default_value_ << "]";
        }
        help << ",  " << arg->description_ << "\n";
    }
    
    if (help_defined_) {
        help << "\n";
        help << "-" << help_short_name_ << ", --" << help_long_name_ << " Display this help and exit\n";
    }
    
    return help.str();
}

bool ArgParser::Parse(int argc, char** argv) {
    auto args = tokenize(argc, argv);
    return Parse(args);
}

bool ArgParser::Parse(const std::vector<std::string>& args) {
    if (args.empty()) {
        return false;
    }

    help_requested_ = false;

    for (size_t i = 1; i < args.size(); ) {
        const auto& arg = args[i];

        if (help_defined_ && (arg == "-" + std::string(1, help_short_name_) || arg == "--" + help_long_name_)) {
            help_requested_ = true;
            return true;
        }
        
        if (!parseArgument(arg, i, args) && !parsePositionalArguments(i, args)) {
            return false;
        }
    }
    
    return validateArguments();
}

bool ArgParser::parseArgument(const std::string& arg, size_t& i, const std::vector<std::string>& args) {
    for (auto& [name, flag] : flag_args_) {
        if (arg == "--" + name || (flag->short_name_ != '\0' && arg == "-" + std::string(1, flag->short_name_))) {
            flag->value_ = true;
            if (flag->stored_value_ != nullptr) {
                *flag->stored_value_ = true;
            }
            i++;
            return true;
        }
    }
    
    for (auto& [name, str_arg] : string_args_) {
        std::string prefix = "--" + name + "=";
        std::string short_prefix = std::string("-") + str_arg->short_name_ + "=";
        
        if ((arg.find(prefix) == 0)) {
            std::string value = arg.substr(prefix.length());
            str_arg->values_.push_back(value);
            if (str_arg->stored_values_ != nullptr) {
                str_arg->stored_values_->push_back(value);
            } else if (str_arg->stored_value_ != nullptr) {
                *str_arg->stored_value_ = value;
            }
            i++;
            return true;
        } else if (str_arg->short_name_ != '\0' && arg.find(short_prefix) == 0) {
            std::string value = arg.substr(short_prefix.length());
            str_arg->values_.push_back(value);
            if (str_arg->stored_values_ != nullptr) {
                str_arg->stored_values_->push_back(value);
            } else if (str_arg->stored_value_ != nullptr) {
                *str_arg->stored_value_ = value;
            }
            i++;
            return true;
        }
    }
    
    for (auto& [name, int_arg] : int_args_) {
        std::string prefix = "--" + name + "=";
        std::string short_prefix = std::string("-") + int_arg->short_name_ + "=";
        
        if ((arg.find(prefix) == 0) || (int_arg->short_name_ != '\0' && arg.find(short_prefix) == 0)) {
            std::string value_str;
            if (arg.find(prefix) == 0) {
                value_str = arg.substr(prefix.length());
            } else {
                value_str = arg.substr(short_prefix.length());
            }
            
            try {
                int value = std::stoi(value_str);
                int_arg->values_.push_back(value);
                if (int_arg->stored_values_ != nullptr) {
                    int_arg->stored_values_->push_back(value);
                } else if (int_arg->stored_value_ != nullptr) {
                    *int_arg->stored_value_ = value;
                }
            } catch (...) {
                return false;
            }
            
            i++;
            return true;
        }
    }
    
    return false;
}

bool ArgParser::parsePositionalArguments(size_t& i, const std::vector<std::string>& args) {
    for (auto& [name, str_arg] : string_args_) {
        if (str_arg->is_positional_ && i < args.size()) {
            str_arg->values_.push_back(args[i]);
            if (str_arg->stored_values_ != nullptr) {
                str_arg->stored_values_->push_back(args[i]);
            } else if (str_arg->stored_value_ != nullptr) {
                *str_arg->stored_value_ = args[i];
            }
            i++;
            return true;
        }
    }
    
    for (auto& [name, int_arg] : int_args_) {
        if (int_arg->is_positional_ && i < args.size()) {
            try {
                int value = std::stoi(args[i]);
                int_arg->values_.push_back(value);
                if (int_arg->stored_values_ != nullptr) {
                    int_arg->stored_values_->push_back(value);
                } else if (int_arg->stored_value_ != nullptr) {
                    *int_arg->stored_value_ = value;
                }
            } catch (...) {
                return false;
            }
            i++;
            return true;
        }
    }
    
    return false;
}

std::vector<std::string> ArgParser::tokenize(int argc, char** argv) {
    std::vector<std::string> args;
    for (int i = 0; i < argc; ++i) {
        args.push_back(argv[i]);
    }
    return args;
}

bool ArgParser::validateArguments() {
    for (const auto& [name, arg] : string_args_) {
        if (!arg->is_positional_ && arg->values_.empty() && !arg->has_default_) {
            return false;
        }
        if (arg->is_multi_value_ && arg->values_.size() < arg->min_args_) {
            return false;
        }
    }
    
    for (const auto& [name, arg] : int_args_) {
        if (!arg->is_positional_ && arg->values_.empty() && !arg->has_default_) {
            return false;
        }
        if (arg->is_multi_value_ && arg->values_.size() < arg->min_args_) {
            return false;
        }
    }
    
    for (auto& [name, flag] : flag_args_) {
        if (flag->has_default_ && !flag->value_) {
            flag->value_ = flag->default_value_;
            if (flag->stored_value_ != nullptr) {
                *flag->stored_value_ = flag->default_value_;
            }
        }
    }
    
    for (auto& [name, str_arg] : string_args_) {
        if (str_arg->has_default_ && str_arg->values_.empty()) {
            str_arg->values_.push_back(str_arg->default_value_);
            if (str_arg->stored_value_ != nullptr) {
                *str_arg->stored_value_ = str_arg->default_value_;
            }
        }
    }
    
    for (auto& [name, int_arg] : int_args_) {
        if (int_arg->has_default_ && int_arg->values_.empty()) {
            int_arg->values_.push_back(int_arg->default_value_);
            if (int_arg->stored_value_ != nullptr) {
                *int_arg->stored_value_ = int_arg->default_value_;
            }
        }
    }
    
    return true;
}

std::string ArgParser::getLongName(char short_name) const {
    auto it = short_to_long_names_.find(short_name);
    if (it != short_to_long_names_.end()) {
        return it->second;
    }
    return "";
}

ArgParser::StringArgument& ArgParser::StringArgument::Default(const std::string& value) {
    has_default_ = true;
    default_value_ = value;
    return *this;
}

ArgParser::StringArgument& ArgParser::StringArgument::StoreValue(std::string& value) {
    stored_value_ = &value;
    return *this;
}

ArgParser::StringArgument& ArgParser::StringArgument::MultiValue(size_t min_args) {
    is_multi_value_ = true;
    min_args_ = min_args;
    return *this;
}

ArgParser::StringArgument& ArgParser::StringArgument::Positional() {
    is_positional_ = true;
    return *this;
}

bool ArgParser::StringArgument::IsPositional() const { return is_positional_; }
const std::string& ArgParser::StringArgument::GetName() const { return long_name_; }
const std::string& ArgParser::StringArgument::GetDescription() const { return description_; }
bool ArgParser::StringArgument::IsMultiValue() const { return is_multi_value_; }
size_t ArgParser::StringArgument::GetMinArgs() const { return min_args_; }
bool ArgParser::StringArgument::HasDefault() const { return has_default_; }
const std::string& ArgParser::StringArgument::GetDefault() const { return default_value_; }
const std::vector<std::string>& ArgParser::StringArgument::GetValues() const { return values_; }
bool ArgParser::StringArgument::IsStored() const { return stored_value_ != nullptr || stored_values_ != nullptr; }
std::string& ArgParser::StringArgument::GetStoredValue() { return *stored_value_; }
std::vector<std::string>& ArgParser::StringArgument::GetStoredValues() { return *stored_values_; }

ArgParser::IntArgument& ArgParser::IntArgument::Default(int value) {
    has_default_ = true;
    default_value_ = value;
    return *this;
}

ArgParser::IntArgument& ArgParser::IntArgument::StoreValue(int& value) {
    stored_value_ = &value;
    return *this;
}

ArgParser::IntArgument& ArgParser::IntArgument::StoreValues(std::vector<int>& values) {
    stored_values_ = &values;
    return *this;
}

ArgParser::IntArgument& ArgParser::IntArgument::MultiValue(size_t min_args) {
    is_multi_value_ = true;
    min_args_ = min_args;
    return *this;
}

ArgParser::IntArgument& ArgParser::IntArgument::Positional() {
    is_positional_ = true;
    return *this;
}

bool ArgParser::IntArgument::IsPositional() const { return is_positional_; }
const std::string& ArgParser::IntArgument::GetName() const { return long_name_; }
const std::string& ArgParser::IntArgument::GetDescription() const { return description_; }
bool ArgParser::IntArgument::IsMultiValue() const { return is_multi_value_; }
size_t ArgParser::IntArgument::GetMinArgs() const { return min_args_; }
bool ArgParser::IntArgument::HasDefault() const { return has_default_; }
int ArgParser::IntArgument::GetDefault() const { return default_value_; }
const std::vector<int>& ArgParser::IntArgument::GetValues() const { return values_; }
bool ArgParser::IntArgument::IsStored() const { return stored_value_ != nullptr || stored_values_ != nullptr; }
int& ArgParser::IntArgument::GetStoredValue() { return *stored_value_; }
std::vector<int>& ArgParser::IntArgument::GetStoredValues() { return *stored_values_; }

ArgParser::Flag& ArgParser::Flag::Default(bool value) {
    has_default_ = true;
    default_value_ = value;
    return *this;
}

ArgParser::Flag& ArgParser::Flag::StoreValue(bool& value) {
    stored_value_ = &value;
    return *this;
}

const std::string& ArgParser::Flag::GetName() const { return long_name_; }
const std::string& ArgParser::Flag::GetDescription() const { return description_; }
bool ArgParser::Flag::HasDefault() const { return has_default_; }
bool ArgParser::Flag::GetDefault() const { return default_value_; }
bool ArgParser::Flag::GetValue() const { return value_; }
bool ArgParser::Flag::IsStored() const { return stored_value_ != nullptr; }
bool& ArgParser::Flag::GetStoredValue() { return *stored_value_; }

}
