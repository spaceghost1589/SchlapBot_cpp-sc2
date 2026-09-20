#include "sc2_utils.h"

#include "sc2api/sc2_common.cc"
#include "sc2api/sc2_map_info.h"
#include "sc2api/sc2_unit.h"

namespace sc2 {

CompareDistance::CompareDistance(const Unit& unit_datum)
    : datum(unit_datum.pos) {};
CompareDistance::CompareDistance(const Point2D& point_datum)
    : datum(point_datum) {};

auto CompareDistance::operator()(const Unit* unit_a, const Unit* unit_b) const
    -> bool {
    return CompareDistance2D(unit_a->pos, unit_b->pos, datum);
}

auto CompareDistance::operator()(const Point2D& point_a,
                                 const Point2D& point_b) const -> bool {
    return CompareDistance2D(point_a, point_b, datum);
}

auto FindRandomLocation(const Point2D& min, const Point2D& max) -> Point2D {
    Point2D target_pos;
    const float playable_w = max.x - min.x;
    const float playable_h = max.y - min.y;
    target_pos.x = (playable_w * GetRandomFraction()) + min.x;
    target_pos.y = (playable_h * GetRandomFraction()) + min.y;
    return target_pos;
}

auto FindRandomLocation(const GameInfo& game_info) -> Point2D {
    return FindRandomLocation(game_info.playable_min, game_info.playable_max);
}

auto FindCenterOfMap(const GameInfo& game_info) -> Point2D {
    Point2D target_pos;
    target_pos.x = game_info.playable_max.x / 2.0F;
    target_pos.y = game_info.playable_max.y / 2.0F;
    return target_pos;
}

}  // namespace sc2
