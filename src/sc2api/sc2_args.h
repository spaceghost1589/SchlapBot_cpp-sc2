#pragma once

#include <span>
#include <string>

#include "sc2_game_settings.h"

namespace sc2 {
auto ParseFromFile(ProcessSettings& process_settings, GameSettings& game_settings, const std::string& file_name)
    -> bool;

/*! @attention Adjust main()'s parameters and the first line after to the following:\n\n
* @code auto main(const int argc, const char* argv[]) -> int {@endcode \n\n
* @code const std::span args{argv, static_cast<size_t>(argc)};@endcode*/
auto ParseSettings(std::span<const char*> args, ProcessSettings& process_settings, GameSettings& game_settings)
    -> bool;
}  // namespace sc2
