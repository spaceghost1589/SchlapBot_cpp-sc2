#pragma once

#include <span>
#include <string>

#include "sc2_game_settings.h"

namespace sc2
{
auto ParseFromFile(ProcessSettings& process_settings, GameSettings& game_settings, const std::string& file_name)
    -> bool;

auto ParseSettings(std::span<const char*> args, ProcessSettings& process_settings, GameSettings& game_settings) -> bool;
} // namespace sc2
