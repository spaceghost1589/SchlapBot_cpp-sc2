#include "resource_manager.h"

#include "sc2api/sc2_agent.h"
#include "sc2api/sc2_common.h"
#include "sc2api/sc2_interfaces.h"
#include "sc2api/sc2_unit.h"
#include "sc2api/sc2_unit_filters.h"

namespace sc2 {

ResourceManager::ResourceManager(const ObservationInterface* Observation, QueryInterface* Query, DebugInterface* Debug)
    : observation(Observation), query(Query), debug(Debug) {
}

// explicit ResourceManager(const Agent* agent) : agent_(*agent) {
//     observation = agent->Observation();
//     query = agent->Query();
//     debug = agent->Debug();

void ResourceManager::Execute() {
    const Units units = observation->GetUnits(Unit::Alliance::Self);

    for (const Unit* unit : units) {
        if (unit->unit_type == UNIT_TYPEID::TERRAN_SCV) {
            if (unit->buffs.empty()) {
                debug->DebugSphereOut((unit->pos), unit->radius);
            } else if (unit->buffs.size() == 1) {
                if (unit->buffs.front() == BUFF_ID::CARRYMINERALFIELDMINERALS)
                    debug->DebugSphereOut(unit->pos, unit->radius, Colors::BlueMinerals);
                else if (unit->buffs.front() == BUFF_ID::CARRYHARVESTABLEVESPENEGEYSERGAS)
                    debug->DebugSphereOut(unit->pos, unit->radius, Colors::GreenVespeneGas);
                else if (unit->buffs.front() == BUFF_ID::CARRYHIGHYIELDMINERALFIELDMINERALS)
                    debug->DebugSphereOut(unit->pos, unit->radius, Colors::GoldMinerals);
                else
                    debug->DebugSphereOut(unit->pos, unit->radius, Colors::PurpleVespeneGas);
            }
        }
    }
    debug->SendDebug();
}
// void ResourceManager::SpeedMineWorker(Unit& worker, speedMiningPositions )
// {
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
//         // Use the computed speed mining position if it exists
//         IF resource exists AND resource is a mineral field THEN
//             target = speedMiningPositions[resource.position]
//
//     // Validate target distance (for example, ensuring the target is not too close or too far)
//     IF target EXISTS AND Distance(worker.position, target) is within desired range THEN
//         MoveWorkerTo(worker, target)
//         // Also ensure the worker continues to gather by re-issuing the gather command (or "smart" command)
//         GatherResource(worker, resource)
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
