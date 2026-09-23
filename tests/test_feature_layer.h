#pragma once

#include <span>

#include "sc2api/sc2_game_settings.h"

namespace sc2 {

bool TestFeatureLayers(std::span<const char*> args);

}
