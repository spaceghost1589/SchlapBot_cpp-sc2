#include <sc2api/sc2_api.h>
#include <sc2api/sc2_unit_filters.h>

#include <iostream>

using namespace sc2;

class Bot : public Agent {
public:
    virtual void OnGameStart() final {
    }

    virtual void OnStep() final {
    }

    virtual void OnUnitIdle(const Unit* unit) final {
    }
};

int main(int argc, char* argv[]) {
    Coordinator coordinator;
    coordinator.LoadSettings(argc, argv);

    Bot bot;
    coordinator.SetParticipants({
        CreateParticipant(Race::Terran, &bot),
        CreateComputer(Race::Zerg)
    });

    coordinator.LaunchStarcraft();
    coordinator.StartGame(sc2::kMapBelShirVestigeLE);

    while (coordinator.Update()) {
    }

    return 0;
}



