#pragma once

#include <span>
#include <string>
#include <string_view>
#include <unordered_map>
#include <vector>

namespace sc2
{

struct Arg {
    std::string_view abbreviation_;
    std::string_view fullname_;
    std::string_view description_;
    bool required_;
};

class ArgParser
{
public:
    ArgParser() = default;
    explicit ArgParser(const std::string_view& executable_name);
    ArgParser(const std::string_view& usage, const std::string_view& description, const std::string_view& example = "");

    auto AddOptions(const std::vector<Arg>& options) -> void;
    auto Parse(std::span<const char*> args) -> bool;

    // If the arg exists returns true and if a value exists for it fill it.
    auto Get(const std::string_view& identifier, std::string& value) -> bool;
    auto PrintHelp() const -> void;
    auto PrintUsage() const -> void;

private:
    std::vector<Arg> options_;
    std::unordered_map<std::string_view, std::string_view> abbv_to_full_;
    std::unordered_map<std::string_view, std::string_view> full_to_value_;

    std::string_view usage_;
    std::string_view description_;
    std::string_view example_;
    std::string_view executable_name_;
};

} // namespace sc2
