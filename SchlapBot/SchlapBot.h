#pragma once

#include <sc2api/sc2_agent.h>

#include <string>
#include <vector>

#include "resource_manager.h"
#include "sc2api/sc2_client.h"
#include "sc2api/sc2_map_info.h"
#include "sc2api/typeids/sc2_5.0.14_typeenums.h"

namespace sc2 {

// The main bot class.
struct SchlapBot : Agent {



    void OnGameFullStart() final;

    void OnGameStart() final;

    void OnStep() final;

    void OnBuildingConstructionComplete(const Unit* building_) final;

    void OnUnitCreated(const Unit* unit_) final;

    void OnUnitIdle(const Unit* unit_) final;

    void OnUnitDestroyed(const Unit* unit_) final;

    void OnUpgradeCompleted(UpgradeID id_) final;

    void OnGameEnd() final;

    void OnError(const std::vector<ClientError>& client_errors,
                 const std::vector<std::string>& protocol_errors = {}) final;
private:


    GameInfo game_info;

    Point3D starting_location_;
    Point2D enemy_start_location_;
};
}  // namespace sc2
