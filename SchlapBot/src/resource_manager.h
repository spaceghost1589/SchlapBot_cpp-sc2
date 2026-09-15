#pragma once
#include "sc2api/sc2_gametypes.h"
#include "sc2api/sc2_interfaces.h"
#include "sc2api/sc2_control_interfaces.h"

namespace sc2 {

struct ResourceManager {

    const ObservationInterface* observation{};
    QueryInterface* query{};
    DebugInterface* debug{};

    ResourceManager() = default;
    // explicit ResourceManager(const Agent* agent);
    ResourceManager(const ObservationInterface* Observation, QueryInterface* Query, DebugInterface* Debug);


    void Execute();

    void SpeedMineWorker();


};
}  // namespace sc2
