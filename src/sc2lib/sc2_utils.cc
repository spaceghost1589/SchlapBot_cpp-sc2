#include "sc2_utils.h"

#include "sc2api/sc2_common.h"
#include "sc2api/sc2_map_info.h"

namespace sc2 {

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
