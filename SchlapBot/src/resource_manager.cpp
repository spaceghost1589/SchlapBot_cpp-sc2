#include "resource_manager.h"

#include <algorithm>
#include <array>
#include <cstdlib>
#include <iostream>
#include <limits>
#include <ranges>
// #include <utility>
#include <vector>

#include "sc2api/sc2_agent.h"
#include "sc2api/sc2_common.h"
#include "sc2api/sc2_interfaces.h"
#include "sc2api/sc2_unit.h"
#include "sc2api/sc2_unit_filters.h"
#include "sc2api/typeids/sc2_5.0.14_typeenums.h"
#include "sc2lib/sc2_search.h"
#include "sc2lib/sc2_utils.h"

namespace sc2 {
namespace {

using namespace std;

} // namespace

ResourceManager::ResourceManager ( Agent* agent )
    : Action ( agent->Actions( ) ),
      Observation ( agent->Observation( ) ),
      Query ( agent->Query( ) ),
      Debug ( agent->Debug( ) ) { }

void ResourceManager::ExecuteStart ( ) {
    expansion_locations = CalculateExpansionLocations (
      Observation,
      Query,
      search::ExpansionParameters { }
    );
    // ranges::sort (expansion_locations,
    // CompareDistance{Observation->GetStartLocation()})

    SetupStaringBase( );

    // SetWorkerCount(units, 8);
    // AssignWorkerStart(units);
}

void ResourceManager::ExecuteStep ( ) const { DebugWorkers( ); } // Execute()

void ResourceManager::DebugWorkers ( ) const {
    for (
      const Units workers =
        Observation->GetUnits ( Unit::Alliance::Self, IsWorker { } );
      const Unit* unit : workers
    )
    {
        if ( IsWorker { }( *unit ) ) {
            if ( unit->buffs.empty( ) ) {
                Debug->DebugSphereOut ( unit->pos, unit->radius );
            } else if ( unit->buffs.size( ) == 1 ) {
                if (
                  unit->buffs.front( ) == BUFF_ID::CARRYMINERALFIELDMINERALS
                )
                {
                    Debug->DebugSphereOut (
                      unit->pos,
                      unit->radius,
                      Colors::BlueMinerals
                    );
                } else if (
                  unit->buffs.front( ) ==
                  BUFF_ID::CARRYHARVESTABLEVESPENEGEYSERGAS
                )
                {
                    Debug->DebugSphereOut (
                      unit->pos,
                      unit->radius,
                      Colors::GreenVespeneGas
                    );
                } else if (
                  unit->buffs.front( ) ==
                  BUFF_ID::CARRYHIGHYIELDMINERALFIELDMINERALS
                )
                {
                    Debug->DebugSphereOut (
                      unit->pos,
                      unit->radius,
                      Colors::GoldMinerals
                    );
                } else {
                    Debug->DebugSphereOut (
                      unit->pos,
                      unit->radius,
                      Colors::PurpleVespeneGas
                    );
                }
            }
        }
    }
}

auto ResourceManager::SetupStaringBase ( ) const -> Expansion {
    Expansion expansion { };

    // Set Starting Townhall.
    if (
      const Units units =
        Observation->GetUnits ( Unit::Alliance::Self, IsBasicTownHall { } );
      units.size( ) == 1
    )
    {
        expansion.townhall = units.front( );
    }

    SetExpansionResource ( expansion );

    AssignWorkerStart ( expansion );

    return expansion;
}

void ResourceManager::SetExpansionResource ( Expansion& expansion ) const {
    // Mineral Fields

    // Set Townhall's local mineral fields...
    Units minerals = Observation->GetUnits (
      Unit::Alliance::Neutral,
      [&] ( const Unit& unit ) -> bool {
        return ( unit.mineral_contents != 0 ) &&
               DistanceSquared2D (
                 expansion.townhall->pos,
                 Observation->GetStartLocation( )
               ) < cluster_distance_squared;
    }
    );
    // ...Sorted by mineral amount.
    ranges::sort ( minerals, CompareMineralAmount { } );
    for ( const Unit* mineral : minerals ) {
        expansion.mineral_assignments.push_back (
          {
              mineral,
              { nullptr, nullptr, nullptr }
        }
        );
    }
    expansion.mineral_assignments.shrink_to_fit( );

    // Vespene Geysers

    // Set Townhall's local Vespene geysers...
    Units vespene_gas = Observation->GetUnits (
      Unit::Alliance::Neutral,
      [&] ( const Unit& unit ) -> bool {
        return ( unit.vespene_contents != 0 ) &&
               DistanceSquared2D (
                 expansion.townhall->pos,
                 Observation->GetStartLocation( )
               ) < cluster_distance_squared;
    }
    );
    // ...Sorted by distance to the townhall.
    ranges::sort (
      vespene_gas,
      CompareDistance { Observation->GetStartLocation( ) }
    );
    for ( const Unit* vespene : vespene_gas ) {
        expansion.vespene_assignments.push_back (
          {
              vespene,
              { nullptr, nullptr, nullptr }
        }
        );
    }
    expansion.vespene_assignments.shrink_to_fit( );
}

void ResourceManager::AssignWorkerStart ( Expansion& expansion ) const {
    Units workers =
      Observation->GetUnits ( Unit::Alliance::Self, IsWorker { } );

    /* Find the closest worker to each mineral patch, based on the
     * farthest-closest mineral-worker pairing. */
    for ( int i = 0; i < expansion.mineral_assignments.size( ); ++i ) {
        float wrk_distance  = std::numeric_limits<float>::max( );
        float far_close_dst = 0.0F;
        int   wkr_select    = 0;
        int   mf_select     = 0;

        for (
          int mf_i = 0; mf_i < expansion.mineral_assignments.size( ); ++mf_i
        )
        {
            const Unit* mf = expansion.mineral_assignments.at ( mf_i ).first;

            if (
              expansion.mineral_assignments.at ( mf_i ).second.front( ) !=
              nullptr
            )
            {
                continue;
            }

            // find the closest worker to the mineral patch
            int wkr_temp_select = 0;
            for ( int wkr_i = 0; wkr_i < workers.size( ); ++wkr_i ) {
                if (
                  const float distance_compare =
                    DistanceSquared2D ( mf->pos, workers.at ( wkr_i )->pos );
                  wrk_distance > distance_compare
                )
                {
                    wrk_distance    = distance_compare;
                    wkr_temp_select = wkr_i;
                }
            }

            /* Check if the worker is the *largest* closest distance from the
             * mineral field. */
            if ( far_close_dst < wrk_distance ) {
                far_close_dst = wrk_distance;
                mf_select     = mf_i;
                wkr_select    = wkr_temp_select;
            }
        }
        expansion.mineral_assignments.at ( mf_select ).second.at ( 0 ) =
          workers.at ( wkr_select );

        // TODO:TESTING
        Action->UnitCommand (
          workers.at ( wkr_select ),
          ABILITY_ID::HARVEST_GATHER,
          expansion.mineral_assignments.at ( mf_select ).first
        );

        workers.erase ( workers.begin( ) + wkr_select );
    }

    for ( int wkr = 0; wkr < workers.size( ); ++wkr ) {
        auto [assign, mf] = std::div (
          wkr,
          static_cast<int> ( expansion.mineral_assignments.size( ) )
        );
        expansion.mineral_assignments.at ( mf ).second.at ( assign ) =
          workers.at ( wkr );
    }
}

void ResourceManager::SetWorkerCount (
  const Units& units,
  const int    count
) const {
    cout << "Starting ResourceManager::SetWorkerCount" << '\n';
    vector<const Unit*> workers;

    while ( workers.size( ) > count ) {
        Debug->DebugKillUnit ( workers.back( ) );
        workers.pop_back( );
    }
}

// void ResourceManager::SpeedMine(Expansion expansion) {
//     for (auto mineral_assignment : expansion.mineral_assignments) {
//         for (auto worker : mineral_assignment.second) {
//             // 0. Check if the worker is in speed mining mode
//             // (e.g., has exactly one order)
//             if (worker->orders.size() > 1) {
//                 continue;
//             }
//             // 1. If the worker is returning with resources
//             if ((worker->orders.front().ability_id ==
//             ABILITY_ID::HARVEST_RETURN) &&
//                 IsCarryingMinerals(*worker)) {
//                 // Compute target near the townhall
//                 Point2D target = FindGatherPoint(expansion.townhall,
//                 mineral_assignment.first, worker);rn BODY; }
//
//
//                     MovePointTowards(townhall.position, worker->position,
//                                      townhall.radius + worker->radius)
//             }
//
//                 Unit& townhall = ClosestTownhallTo(worker)
//
//         }
//     }
//
//     expansion.mineral_assignments
//
//         Point2D target;
//     UnitOrder& current_order = worker.orders.front().ability_id;
//
//     // 2. If the worker is gathering minerals
//     elseif(current_order.ability_id == HARVEST_GATHER) Unit& resource =
//         current_order.target_unit_tag();
//
//     // 3. Use the computed speed mining position if it exists
//     IF resource exists AND resource is a mineral field THEN
//         target = speedMiningPositions[resource.position]
//
// // Validate target distance (for example, ensuring the target is not too
// close or too far) IF target EXISTS AND Distance(worker.position, target) is
// within desired range THEN
//     MoveWorkerTo(worker, target)
//     // Also ensure the worker continues to gather by re-issuing the gather
//     command (or "smart" command) GatherResource(worker, resource)
// }

} // namespace sc2

// FUNCTION SpeedMineWorker(worker, speedMiningPositions):
// // Check if the worker is in speed mining mode (e.g., has exactly one order)
// IF worker.orderCount == 1 THEN
// target = NULL
//     // If the worker is returning with resources (but not gas)
//     IF worker.isReturning AND NOT worker.isCarryingGas THEN
//         townhall = ClosestTownhallTo(worker)
//         // Compute target near the townhall
//         target = MovePointTowards(townhall.position, worker.position,
//         townhall.radius + worker.radius)
//
//     // If the worker is gathering minerals
//     ELSE IF worker.isGathering THEN
//         resource = GetResourceFromWorkerOrder(worker)
//         // Use the computed speed mining position if it exists
//         IF resource exists AND resource is a mineral field THEN
//             target = speedMiningPositions[resource.position]
//
//     // Validate target distance (for example, ensuring the target is not too
//     close or too far) IF target EXISTS AND Distance(worker.position, target)
//     is within desired range THEN
//         MoveWorkerTo(worker, target)
//         // Also ensure the worker continues to gather by re-issuing the
//         gather command (or "smart" command) GatherResource(worker, resource)

// FUNCTION ComputeSpeedMiningPositions(expansions, workerRadius):
// // Create a mapping from each mineral patch position to its target position.
// positionsMapping = empty map
// FOR EACH (base, resourceList) IN expansions:
//     FOR EACH resource IN resourceList:
//         // Calculate the ideal mining distance.
//         miningRadius = resource.radius + workerRadius
//         // Compute the target position by moving from the mineral patch
//         towards the base. target = MovePointTowards(resource.position, base,
//         miningRadius)
//
//         positionsMapping[resource.position] = target
//
// RETURN positionsMapping
