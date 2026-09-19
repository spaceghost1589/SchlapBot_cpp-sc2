#include <sc2api/sc2_agent.h>
#include <sc2api/sc2_api.h>
#include <sc2api/sc2_unit_filters.h>

#include <iostream>

using namespace sc2;

class DB_Bot : public Agent {
public:
    bool do_thing = {true};
    bool do_thing2 = {true};

    void OnGameStart() final {
    }

    virtual void OnStep() final {
        Units units = Observation()->GetUnits(Unit::Self);

        const Unit* CC = *std::ranges::find_if(
            units, [&](const Unit* unit) -> bool { return unit->unit_type == UNIT_TYPEID::TERRAN_COMMANDCENTER; });

        Units workers;
        for (const Unit* unit : units) {
            if (unit->unit_type == UNIT_TYPEID::TERRAN_SCV)
                workers.push_back(unit);
        }

        if (do_thing) {
            for (AvailableAbility ability : Query()->GetAbilitiesForUnit(CC, true, true).abilities) {
                std::cout << ability.ability_id.TypeToName() << " : " << ability.ability_id.to_string() << '\n';
            }
            for (AvailableAbility ability : Query()->GetAbilitiesForUnit(CC, true, false).abilities) {
                std::cout << ability.ability_id.TypeToName() << " : " << ability.ability_id.to_string() << '\n';
            }
            Actions()->UnitCommand(CC, ABILITY_ID::LOADALL);
            do_thing = false;
        } else if (CC->cargo_space_taken != 0 && do_thing2) {
            // AvailableAbilities available_abilities = Query()->GetAbilitiesForUnit(CC, true, false);
            for (AvailableAbility ability : Query()->GetAbilitiesForUnit(CC, true, true).abilities) {
                std::cout << ability.ability_id.TypeToName() << " : " << ability.ability_id.to_string() << '\n';
            }
            for (AvailableAbility ability : Query()->GetAbilitiesForUnit(CC, true, false).abilities) {
                std::cout << ability.ability_id.TypeToName() << " : " << ability.ability_id.to_string() << '\n';
            }
            do_thing2 = false;
        }
    }


    void OnUnitIdle(const Unit* unit) final {
}
}
;

auto main(int argc, char* argv[]) -> int {
    Coordinator coordinator;
    coordinator.LoadSettings(argc, argv);

    coordinator.SetRealtime(true);

    DB_Bot db_bot;
    coordinator.SetParticipants({CreateParticipant(Race::Terran, &db_bot, "DB_Bot"),
                                 CreateComputer(Race::Random, Difficulty::VeryEasy, AIBuild::Macro)});

    coordinator.LaunchStarcraft();
    coordinator.StartGame(sc2::kMapBelShirVestigeLE);

    while (coordinator.Update()) {
    }

    return 0;
}
