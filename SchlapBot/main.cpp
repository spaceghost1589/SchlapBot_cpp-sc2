#include <cstddef>
#include <iostream>

#include "SchlapBot.h"
#include "sc2api/sc2_common.cc"
#include "sc2api/sc2_coordinator.h"
#include "sc2api/sc2_gametypes.h"

namespace {
using namespace std;
} // namespace

static std::string GetRandomMap ( ) {
    const int random_map = sc2::GetRandomInteger ( 1, 7 );

    if ( random_map == 1 ) {
        return "IncorporealAIE_v4.SC2Map";
    }
    if ( random_map == 2 ) {
        return "LeyLinesAIE_v3.SC2Map";
    }
    if ( random_map == 3 ) {
        return "MagannathaAIE_v2.SC2Map";
    }
    if ( random_map == 4 ) {
        return "PersephoneAIE_v4.SC2Map";
    }
    if ( random_map == 5 ) {
        return "PylonAIE_v4.SC2Map";
    }
    if ( random_map == 6 ) {
        return "TorchesAIE_v4.SC2Map";
    } // 7
    return "UltraloveAIE_v2.SC2Map";
}
#ifdef BUILD_FOR_LADDER
namespace {
} // namespace

int main ( const int argc, char* const argv[] ) {
    Options options;
    ParseArguments ( argc, argv, &options );

    sc2::Coordinator coordinator;
    Bot              bot;

    size_t num_agents = 2;
    coordinator.SetParticipants (
      { CreateParticipant ( sc2::Race::Random, &bot, "BlankBot" ) }
    );

    std::cout << "Connecting to port " << options.GamePort << std::endl;
    coordinator.Connect ( options.GamePort );
    coordinator.SetupPorts ( num_agents, options.StartPort, false );

    // NB (alkurbatov): Increase speed of steps processing.
    // Disables ability to control your bot during game.
    // Recommended for competitions.
    coordinator.SetRawAffectsSelection ( true );

    coordinator.JoinGame( );
    coordinator.SetTimeoutMS ( 10'000 );
    std::cout << "Successfully joined game" << std::endl;

    while ( coordinator.Update( ) ) { }

    return 0;
} // main Ladder

#else

int main (
  const int argc,
  char*     argv[]
) // NOLINT(*-avoid-c-arrays, *-use-internal-linkage)
{
    sc2::Coordinator coordinator;
    coordinator.LoadSettings ( argc, argv );

    // NOTE: Uncomment to start the game in full screen mode.
    // coordinator.SetFullScreen(true);

    // NOTE: Uncomment to play at normal speed.
    coordinator.SetRealtime ( true );

    sc2::SchlapBot SchlapBot { };

    coordinator.SetParticipants (
      {
          CreateParticipant ( sc2::Race::Terran, &SchlapBot, "SchlapBot" ),
          CreateComputer (
            sc2::Race::Random,
            sc2::Difficulty::Easy,
            sc2::AIBuild::Macro
          ),
      }
    );


    // const MapData map_data("IncorporealAIE_v4");

    coordinator.LaunchStarcraft( );

    coordinator.StartGame ( GetRandomMap() );

    while ( coordinator.Update( ) ) { }

    return 0;

    // const span args(argv, static_cast<size_t>(argc));
    //
    // if (args.size() < 2) {
    //     cerr << "IncorporealAIE_v4" << '\n';
    //     // return 1;
    // }
    //
    // sc2::Coordinator coordinator;
    //
    // if (coordinator.LoadSettings(args)) {
    //     cout << "LoadSettings success." << '\n';
    // } else {
    //     SRC_LocationOut("LoadSettings failed.");
    //     abort();
    // }
    //
    // coordinator.SetMultithreaded(true);
    // cout << "Multithreaded set." << '\n';
    //
    //
    // constexpr bool realtime = true;
    // coordinator.SetRealtime(realtime);
    // SRC_LocationOut(format("Realtime set: {}", realtime).c_str());
    //

} // Main (Local)
#endif
