#pragma once
#include "sc2api/sc2_gametypes.h"
#include "sc2api/sc2_interfaces.h"
#include "sc2api/sc2_control_interfaces.h"
#include "sc2api/sc2_unit.h"

namespace sc2 {

namespace Colors {
static const Color BlueMinerals = Color(112, 164, 219);
static const Color GoldMinerals = Color(238, 178, 62);
static const Color GreenVespeneGas = Color(74, 211, 78);
static const Color PurpleVespeneGas = Color(131, 56, 218);
}

struct ResourceManager {

    ResourceManager() = default;
    explicit ResourceManager(Agent* agent);

    void Execute();
    void SetWorkerCount(const Units& units, int count);
    void AssignWorkerStart(const Units& units);

    void SpeedMineWorker();

private:
    ActionInterface* action;
    const ObservationInterface* observation;
    QueryInterface* query;
    DebugInterface* debug;
};
}  // namespace sc2
