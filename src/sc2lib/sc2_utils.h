#pragma once

#include "sc2api/sc2_common.h"
#include "sc2api/sc2_map_info.h"
#include "sc2api/sc2_unit.h"

namespace sc2 {

struct CompareDistance {
    explicit CompareDistance(const Unit& unit_datum);
    explicit CompareDistance(const Point2D& point_datum);

    auto operator()(const Unit* unit_a, const Unit* unit_b) const -> bool;
    auto operator()(const Point2D& point_a, const Point2D& point_b) const
        -> bool;

private:
    Point2D datum;
};  // CompareDistance

auto FindRandomLocation(const Point2D& min, const Point2D& max) -> Point2D;
auto FindRandomLocation(const GameInfo& game_info) -> Point2D;
auto FindCenterOfMap(const GameInfo& game_info) -> Point2D;

}  // namespace sc2
