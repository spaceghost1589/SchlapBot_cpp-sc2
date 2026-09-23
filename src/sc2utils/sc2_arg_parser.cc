#include "sc2_arg_parser.h"

#include <algorithm>
#include <cstddef>
#include <iostream>
#include <span>
#include <string>
#include <string_view>
#include <vector>

namespace sc2
{

ArgParser::ArgParser(const std::string_view& executable_name) : executable_name_(executable_name)
{
}

ArgParser::ArgParser(const std::string_view& usage, const std::string_view& description,
                     const std::string_view& example)
    : usage_(usage), description_(description), example_(example)
{
}

auto ArgParser::AddOptions(const std::vector<Arg>& options) -> void
{
    for (const auto& o : options) {
        options_.push_back(o);
        abbv_to_full_[o.abbreviation_] = o.fullname_;
    }
}

auto ArgParser::Parse(std::span<const char*> args) -> bool
{
    // Prevents mass NOLINT comments from C-style array access.
    const std::vector<std::string_view> arguments(args.begin(), args.end());

    // If help is sent print out help and return false, don't parse.
    if (std::ranges::any_of(arguments, [&](const std::string_view arg) -> bool {
            return arg == "-h" || arg == "--help";
        })) {
        PrintHelp();
        return false;
    }

    executable_name_ = arguments.front();

    for (std::size_t i = 0; i < arguments.size(); ++i) {
        const std::string_view arg_i = arguments.at(i);

        // Skip values.
        if (arg_i.front() != '-') {
            continue;
        }

        // Check that it's a valid option.
        if (std::ranges::none_of(options_, [&](const Arg& opt) -> bool {
                return opt.abbreviation_ == arg_i || opt.fullname_ == arg_i;
            })) {
            std::cerr << arg_i << " is an unrecognized argument." << '\n';
            return false;
        }

        // Determine if `arg_i` is a fullname or an abbreviation.
        std::string_view fullname = arg_i;
        // If it's an abbreviation, get the fullname.
        if (fullname.at(1) != '-') {
            fullname = abbv_to_full_[fullname];
        }

        std::string_view value;
        if (i + 1 < arguments.size()) {
            if (const std::string_view next_arg = arguments.at(i + 1); !next_arg.empty() && next_arg.front() != '-') {
                value = next_arg;
            }
        }

        // Remove the dashes.
        full_to_value_[fullname.substr(2)] = value;
    }

    // Verify we have all required options.
    return std::ranges::all_of(options_, [&](const Arg& opt) -> bool {
        return !opt.required_ || full_to_value_.contains(opt.fullname_.substr(2));
    });
}

auto ArgParser::Get(const std::string_view& identifier, std::string_view& value) -> bool
{
    std::string_view fullname = identifier;

    // If the identifier is the abbreviation turn it into the fullname
    if (fullname.size() == 1) {
        if (const auto it = abbv_to_full_.find('-' + std::string(identifier)); it != abbv_to_full_.end()) {
            fullname = it->second;
        } else {
            return false;
        }
    }

    if (fullname.front() == '-') {
        fullname = fullname.substr(2);
    }

    if (const auto it = full_to_value_.find(fullname); it != full_to_value_.end()) {
        value = it->second;
    } else {
        return false;
    }

    return true;
}

auto ArgParser::PrintHelp() const -> void
{
    PrintUsage();
    std::cout << "Options -" << '\n';
    for (const auto& o : options_) {
        std::cout << "  " << o.abbreviation_ << ", " << o.fullname_ << " " << o.description_ << '\n';
    }
}

auto ArgParser::PrintUsage() const -> void
{
    std::cout << "Usage: " << executable_name_ << " ";
    // Append required arguments.
    for (const auto& o : options_) {
        if (o.required_) {
            std::cout << o.abbreviation_ << " [" << o.fullname_.substr(2) << "] ";
        }
    }
    std::cout << "\n\n";
}

} // namespace sc2
