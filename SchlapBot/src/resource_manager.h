#pragma once
#include <flat_map>
#include "sc2api/sc2_common.h"
#include "sc2api/sc2_gametypes.h"
#include "sc2api/sc2_interfaces.h"
#include "sc2api/sc2_unit.h"

namespace sc2 {

namespace Colors {
static const Color BlueMinerals = Color(112, 164, 219);
static const Color GoldMinerals = Color(238, 178, 62);
static const Color GreenVespeneGas = Color(74, 211, 78);
static const Color PurpleVespeneGas = Color(131, 56, 218);
}  // namespace Colors

/*! Key - @c Resource (Mineral Patch / Vespene Geyser)\n
 * Array - @c Workers assigned to the resource */
using ResourceWorkerAssignment = std::pair<const Unit* /*resource*/, std::array<const Unit* /*workers*/, 3>>;
using ExpansionResources = std::vector<ResourceWorkers>;

struct ResourceManager {
    const Unit* starting_townhall{};
    std::vector<Point3D> expansion_locations;

    ResourceManager() = default;
    explicit ResourceManager(Agent* agent);

    void ExecuteStart();
    void ExecuteStep();

    ExpansionResources BuildResourceToWorker(const Unit* unit, const Units* units);

    void SetWorkerCount(const Units& units, int count) const;
    void AssignWorkerStart(const Units& units);
    // void AssignWorkerStart_SpeedTest(const Units& units);

    // void SpeedMineWorker(std::array<const Unit*, 3>);

    // ExpansionResources BuildResourceToWorker (const Unit* townhall);



    struct Expansion {
        const Unit* townhall;
        std::vector<const Unit*> workers;
        //! Bunkers & Nydus
        std::vector<const Unit*> transporter;
        std::vector<const Unit*> minerals;
        //! Geyser & Refinery / Extractor / Assimilator
        std::vector<std::pair<const Unit*, const Unit*>> vespene;

        std::vector<ResourceWorkerAssignment> resource_worker_assignment;
    };  // struct Expansion

    std::flat_map<Point3D, Expansion>

private:
    ActionInterface* Action;
    const ObservationInterface* Observation;
    QueryInterface* Query;
    DebugInterface* Debug;

    struct CompareMineralAmount {
        auto operator()(const Unit* mineral_patch_a, const Unit* mineral_patch_b) const -> bool {
            return static_cast<int>(mineral_patch_a->mineral_contents) >
                   static_cast<int>(mineral_patch_b->mineral_contents);
        }
    };
};  // struct ResourceManager
}  // namespace sc2
