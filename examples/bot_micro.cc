#include <cstddef>
#include <span>

#include "bot_examples.h"
#include "sc2api/sc2_api.h"
#include "sc2utils/sc2_arg_parser.h"
#include "sc2utils/sc2_manage_process.h"

//*************************************************************************************************
auto main(const int argc, const char* argv[]) -> int {
    const std::span args{argv, static_cast<size_t>(argc)};

    sc2::Coordinator coordinator;
    if (!coordinator.LoadSettings(args)) {
        return 1;
    }

    coordinator.SetRealtime(true);

    // Add the custom bot, it will control the player.
    sc2::MarineMicroBot bot;
    coordinator.SetParticipants({
        CreateParticipant(sc2::Race::Terran, &bot),
        CreateComputer(sc2::Race::Zerg),
    });

    // Start the game.
    coordinator.LaunchStarcraft();
    coordinator.StartGame(sc2::kMapMarineMicro);

    while (coordinator.Update()) {
        if (sc2::PollKeyPress()) {
            break;
        }
    }

    return 0;
}
