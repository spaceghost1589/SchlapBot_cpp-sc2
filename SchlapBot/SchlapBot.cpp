#include "SchlapBot.h"

#include <stdint.h>

#include <algorithm>
#include <chrono>
#include <cstdint>
#include <fstream>
#include <iostream>
#include <queue>
#include <ranges>
#include <string>
#include <vector>

#include "sc2api/sc2_client.h"
#include "sc2api/sc2_common.h"
#include "sc2api/sc2_control_interfaces.h"
#include "sc2api/sc2_gametypes.h"
#include "sc2api/sc2_interfaces.h"
#include "sc2api/sc2_map_info.h"
#include "sc2api/sc2_unit.h"
#include "sc2api/typeids/sc2_5.0.14_typeenums.h"
#include "sc2lib/sc2_search.h"
#include "sc2lib/sc2_utils.h"
#include "resource_manager.h"

namespace sc2 {

using enum Unit::Alliance;

ResourceManager resource_manager;

// The main bot class.
GameInfo game_info;

Point3D starting_location_;
Point2D enemy_start_location_;
std::vector<Point3D> expansions;

void SchlapBot::OnGameFullStart() {
}

void SchlapBot::OnGameStart() {
    std::cout << '\n' << "OnGameStart() -begin-" << '\n' << std::endl;

     resource_manager = ResourceManager(Observation(), Query(), Debug());

    game_info = Observation()->GetGameInfo();
    starting_location_ = Observation()->GetStartLocation();
    enemy_start_location_ = Observation()->GetGameInfo().enemy_start_locations.at(0);

    std::cout << "GetStartLocation finished" << '\n';

    expansions = search::CalculateExpansionLocations(Observation(), Query(), search::ExpansionParameters{});
}
void SchlapBot::OnStep() {
    uint32_t game_loop = Observation()->GetGameLoop();

    Units units = Observation()->GetUnits(Self);


    resource_manager.Execute();
}

//------------------------------------------------------------------------------

//------------------------------------------------------------------------------

void SchlapBot::OnBuildingConstructionComplete(const Unit* building_) {
    std::cout << UnitTypeToName(building_->unit_type) << "(" << building_->tag << ") constructed" << '\n';
}

void SchlapBot::OnUnitCreated(const Unit* unit_) {
    // std::cout << UnitTypeToName(unit_->unit_type) << "(" << unit_->tag << ") was created" << '\n';
}

void SchlapBot::OnUnitIdle(const Unit* unit_) {
    // std::cout << UnitTypeToName(unit_->unit_type) << "(" << unit_->tag << ") is idle" << '\n';
}

void SchlapBot::OnUnitDestroyed(const Unit* unit_) {
    std::cout << UnitTypeToName(unit_->unit_type) << "(" << unit_->tag << ") was destroyed" << '\n';
}

void SchlapBot::OnUpgradeCompleted(const UpgradeID id_) {
    std::cout << UpgradeIDToName(id_) << " completed" << '\n';
}

void SchlapBot::OnGameEnd() {
    std::cout << "Game over! You " << GameResultToName(Observation()->GetResults().front().result) << "!" << '\n';
}

void SchlapBot::OnError(const std::vector<ClientError>& client_errors,
                        const std::vector<std::string>& protocol_errors) {
    for (const auto err : client_errors) {
        std::cerr << "Encountered client error: " << static_cast<int>(err) << '\n';
    }

    for (const auto& err : protocol_errors) {
        std::cerr << "Encountered protocol error: " << err << '\n';
    }
}

}  // namespace sc2
