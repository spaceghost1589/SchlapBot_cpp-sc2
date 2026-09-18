#include "resource_manager.h"

#include <algorithm>
#include <iostream>
#include <queue>
#include <vector>

#include "sc2api/sc2_agent.h"
#include "sc2api/sc2_common.h"
#include "sc2api/sc2_interfaces.h"
#include "sc2api/sc2_unit.h"
#include "sc2api/typeids/sc2_5.0.14_typeenums.h"
#include "sc2lib/sc2_search.h"

namespace {
using namespace std;
}  // namespace

namespace sc2 {

ResourceManager::ResourceManager(Agent* agent) {
    Action = agent->Actions();
    Observation = agent->Observation(), Query = agent->Query(), Debug = agent->Debug();
}

// explicit ResourceManager(const Agent* agent) : agent_(*agent) {
//     observation = agent->Observation();
//     query = agent->Query();
//     debug = agent->Debug();

void ResourceManager::ExecuteStart() {
    expansion_locations = search::CalculateExpansionLocations(Observation, Query, search::ExpansionParameters{});

    Units units = Observation->GetUnits(Unit::Alliance::Self);

    starting_townhall = *ranges::find_if(units, [&](const Unit* unit) {
        return unit->unit_type == UNIT_TYPEID::TERRAN_COMMANDCENTER || unit->unit_type == UNIT_TYPEID::PROTOSS_NEXUS ||
               unit->unit_type == UNIT_TYPEID::ZERG_HATCHERY;
    });

    BuildResourceToWorker(starting_townhall, &units);

    // SetWorkerCount(units, 8);
    // AssignWorkerStart(units);
}

void ResourceManager::ExecuteStep() {
    const Units& workers = Observation->GetUnits(
        Unit::Alliance::Self, [&](const Unit& unit) { return unit.unit_type == UNIT_TYPEID::TERRAN_SCV; });
    const Units& resources = Observation->GetUnits(
        [&](const Unit& unit) -> bool { return unit.mineral_contents > 0 || unit.vespene_contents > 0; });

    for (const Unit* unit : workers) {
        if (unit->unit_type == UNIT_TYPEID::TERRAN_SCV) {
            if (unit->buffs.empty()) {
                Debug->DebugSphereOut(unit->pos, unit->radius);
            } else if (unit->buffs.size() == 1) {
                if (unit->buffs.front() == BUFF_ID::CARRYMINERALFIELDMINERALS)
                    Debug->DebugSphereOut(unit->pos, unit->radius, Colors::BlueMinerals);
                else if (unit->buffs.front() == BUFF_ID::CARRYHARVESTABLEVESPENEGEYSERGAS)
                    Debug->DebugSphereOut(unit->pos, unit->radius, Colors::GreenVespeneGas);
                else if (unit->buffs.front() == BUFF_ID::CARRYHIGHYIELDMINERALFIELDMINERALS)
                    Debug->DebugSphereOut(unit->pos, unit->radius, Colors::GoldMinerals);
                else
                    Debug->DebugSphereOut(unit->pos, unit->radius, Colors::PurpleVespeneGas);
            }
        }
    }
}  // Execute()

void ResourceManager::SetWorkerCount(const Units& units, const int count) const {
    cout << "Starting ResourceManager::SetWorkerCount" << '\n';
    vector<const Unit*> workers;
    for (const auto* unit : units) {
        if (unit->unit_type == UNIT_TYPEID::TERRAN_SCV) {
            workers.push_back(unit);
        }
    }
    while (workers.size() > count) {
        Debug->DebugKillUnit(workers.back());
        workers.pop_back();
    }
}

void ResourceManager::AssignWorkerStart(const Units& units) {
    cout << "Starting ResourceManager::AssignWorkerStart" << '\n';
    Units workers;
    for (const Unit* unit : units) {
        if (unit->unit_type == UNIT_TYPEID::TERRAN_SCV) {
            workers.push_back(unit);
        }
    }

    Units minerals = Observation->GetUnits(Unit::Alliance::Neutral, [&](const Unit& unit) -> bool {
        return unit.mineral_contents != 0 && Distance2D(unit.pos, Observation->GetStartLocation()) < 10;
    });

    ranges::sort(minerals, CompareMineralAmount{});

    for (int i = 0; i < workers.size(); ++i) {
        Action->UnitCommand(workers.at(i), ABILITY_ID::HARVEST_GATHER, minerals.at(i % minerals.size()));
    }
}

ExpansionResources ResourceManager::BuildResourceToWorker(const Unit* townhall, const Units* units) {
    Units workers;
    for (auto unit : *units) {
        if (unit->unit_type == UNIT_TYPEID::TERRAN_SCV || unit->unit_type == UNIT_TYPEID::TERRAN_MULE ||
                unit->unit_type == UNIT_TYPEID::PROTOSS_PROBE || unit->unit_type == UNIT_TYPEID::ZERG_DRONE ||
                unit->unit_type == UNIT_TYPEID::ZERG_DRONEBURROWED)
            workers.push_back(unit);
    }

    Units minerals = Observation->GetUnits(Unit::Alliance::Neutral, [&](const Unit& unit) -> bool {
        return unit.mineral_contents != 0 &&
               DistanceSquared2D(townhall->pos, Observation->GetStartLocation()) < 100 /* 10^2 */;
    });
    ranges::sort(minerals, CompareMineralAmount{});

    for (int i = 0; i < workers.size(); ++i) {
        Action->UnitCommand(workers.at(i), ABILITY_ID::HARVEST_GATHER, minerals.at(i % minerals.size()));
    }
    return;
}

// void ResourceManager::SpeedMineWorker(array<const Unit*, 3> workers) {
//     // Check if the worker is in speed mining mode (e.g., has exactly one order)
//     if (worker.orders.size()> 1)
//         return;
//
//     Point2D target;
//     UnitOrder& current_order = worker.orders.front().ability_id;
//
//     // If the worker is returning with resources (but not gas)
//     if ((current_order.ability_id == HARVEST_RETURN) && IsCarryingMinerals(worker))
//
//         Unit& townhall = ClosestTownhallTo(worker)
//         // Compute target near the townhall
//         Point2D target = MovePointTowards(townhall.position, worker.position, townhall.radius + worker.radius)
//
//     // If the worker is gathering minerals
//     elseif (current_order.ability_id == HARVEST_GATHER)
//         Unit& resource = current_order.target_unit_tag();
//
//     // Use the computed speed mining position if it exists
//     IF resource exists AND resource is a mineral field THEN
//         target = speedMiningPositions[resource.position]
//
// // Validate target distance (for example, ensuring the target is not too close or too far)
// IF target EXISTS AND Distance(worker.position, target) is within desired range THEN
//     MoveWorkerTo(worker, target)
//     // Also ensure the worker continues to gather by re-issuing the gather command (or "smart" command)
//     GatherResource(worker, resource)
// }

}  // namespace sc2

// FUNCTION SpeedMineWorker(worker, speedMiningPositions):
// // Check if the worker is in speed mining mode (e.g., has exactly one order)
// IF worker.orderCount == 1 THEN
// target = NULL
//     // If the worker is returning with resources (but not gas)
//     IF worker.isReturning AND NOT worker.isCarryingGas THEN
//         townhall = ClosestTownhallTo(worker)
//         // Compute target near the townhall
//         target = MovePointTowards(townhall.position, worker.position, townhall.radius + worker.radius)
//
//     // If the worker is gathering minerals
//     ELSE IF worker.isGathering THEN
//         resource = GetResourceFromWorkerOrder(worker)
//         // Use the computed speed mining position if it exists
//         IF resource exists AND resource is a mineral field THEN
//             target = speedMiningPositions[resource.position]
//
//     // Validate target distance (for example, ensuring the target is not too close or too far)
//     IF target EXISTS AND Distance(worker.position, target) is within desired range THEN
//         MoveWorkerTo(worker, target)
//         // Also ensure the worker continues to gather by re-issuing the gather command (or "smart" command)
//         GatherResource(worker, resource)

// FUNCTION ComputeSpeedMiningPositions(expansions, workerRadius):
// // Create a mapping from each mineral patch position to its target position.
// positionsMapping = empty map
// FOR EACH (base, resourceList) IN expansions:
//     FOR EACH resource IN resourceList:
//         // Calculate the ideal mining distance.
//         miningRadius = resource.radius + workerRadius
//         // Compute the target position by moving from the mineral patch towards the base.
//         target = MovePointTowards(resource.position, base, miningRadius)
//
//         positionsMapping[resource.position] = target
//
// RETURN positionsMapping
