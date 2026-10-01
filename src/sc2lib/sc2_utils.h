#pragma once

#include "sc2api/sc2_common.h"
#include "sc2api/sc2_map_info.h"

namespace sc2 {

auto FindRandomLocation(const Point2D& min, const Point2D& max) -> Point2D;
auto FindRandomLocation(const GameInfo& game_info) -> Point2D;
auto FindCenterOfMap(const GameInfo& game_info) -> Point2D;

}  // namespace sc2
