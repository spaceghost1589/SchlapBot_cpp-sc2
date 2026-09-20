#pragma once

#include "sc2api/sc2_common.h"
#include "sc2api/sc2_map_info.h"
#include "sc2api/sc2_unit.h"

namespace sc2 {

struct CompareDistance
{
    explicit CompareDistance ( const Unit& unit_datum );
    explicit CompareDistance ( const Point2D& point_datum );

    auto operator ( ) ( const Unit* unit_a, const Unit* unit_b ) const -> bool;
    auto operator ( ) ( const Point2D& point_a, const Point2D& point_b ) const
      -> bool;

private:
    Point2D datum;
}; // CompareDistance

constexpr float worker_radius   = 0.375F;
constexpr float townhall_radius = 2.5F;
constexpr float vespene_radius  = 1.5F;
constexpr float mineral_radius  = 1.125F;

//! Finds the contact point between a worker and a townhall or resource patch.
auto FindWorkerContactPoint (
  const Unit& departure,
  const Unit& destination,
  float       offset_length = 0.0F
) -> Point2D;
auto FindMineralGatherPoint ( const Unit& mineral, const Unit& townhall )
  -> Point2D;
auto FindVespeneGatherPoint ( const Unit& vespene, const Unit& townhall )
  -> Point2D;
auto FindTownhallDeliveryPoint ( const Unit& townhall, const Unit& resource )
  -> Point2D;

auto FindRandomLocation ( const Point2D& min, const Point2D& max ) -> Point2D;
auto FindRandomLocation ( const GameInfo& game_info ) -> Point2D;
auto FindCenterOfMap ( const GameInfo& game_info ) -> Point2D;

} // namespace sc2
