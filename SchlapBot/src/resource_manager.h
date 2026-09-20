#pragma once
#include <array>
#include <utility>
#include <vector>

#include "sc2api/sc2_common.h"
#include "sc2api/sc2_gametypes.h"
#include "sc2api/sc2_interfaces.h"
#include "sc2api/sc2_unit.h"
#include "sc2api/typeids/sc2_5.0.14_typeenums.h"

namespace sc2 {

namespace Colors {
static constexpr Color BlueMinerals = {112, 164, 219};
static constexpr Color GoldMinerals = {238, 178, 62};
static constexpr Color GreenVespeneGas = {74, 211, 78};
static constexpr Color PurpleVespeneGas = {131, 56, 218};
}  // namespace Colors

using Resource = Unit;
using Worker = Unit;
/*! @c Resource (Mineral Patch or Vespene Geyser)\n
 * @c array of @c Worker assigned to the resource. */
using WorkerAssignment = std::pair<const Resource*, std::array<const Worker*, 3>>;
using ExpansionResources = std::vector<WorkerAssignment>;

struct Expansion {
    const Unit* townhall;
    //! Bunkers & Nydus
    std::vector<const Unit*> transporter;
    //! Geyser & Refinery / Extractor / Assimilator
    ExpansionResources mineral_assignments;
    ExpansionResources vespene_assignments;
};  // struct Expansion

struct ResourceManager {
    ResourceManager() = default;
    explicit ResourceManager(Agent* agent);

    void ExecuteStart();
    void ExecuteStep() const;
    void DebugWorkers() const;

    auto SetupStaringBase() const -> Expansion;

    void SetExpansionResource(Expansion& expansion) const;
    void BuildResourceToWorker(Expansion& expansion);

    void SetWorkerCount(const Units& units, int count) const;
    void AssignWorkerStart(Expansion& expansion) const;
    // void AssignWorkerStart_SpeedTest(const Units& units);

    // void ResourceManager::SpeedMine(Expansion expansion);

    // ExpansionResources BuildResourceToWorker (const Unit* townhall);

    // std::flat_map<Point3D, Expansion>

private:
    ActionInterface* Action{};
    const ObservationInterface* Observation{};
    QueryInterface* Query{};
    DebugInterface* Debug{};

    const Unit* starting_townhall{};
    std::vector<Point3D> expansion_locations;

    // 10^2
    float cluster_distance_squared = 100.0F;

    struct CompareMineralAmount {
        auto operator()(const Unit* mineral_patch_a,
                        const Unit* mineral_patch_b) const -> bool {
            return static_cast<int>(mineral_patch_a->mineral_contents) >
                   static_cast<int>(mineral_patch_b->mineral_contents);
        }
    };

    struct CompareVespeneAmount {
        auto operator()(const Unit& vespene_geyser_a,
                        const Unit& vespene_geyser_b) const -> bool {
            return static_cast<int>(vespene_geyser_a.vespene_contents) >
                   static_cast<int>(vespene_geyser_b.vespene_contents);
        }
    };

};  // struct ResourceManager
}  // namespace sc2
