/*! \file sc2_gametypes.h
    \brief Types used in setting up a game.
*/
#pragma once

#include <cstdint>
#include <string>
#include <utility>
#include <vector>

// #include "sc2_common.h"

namespace sc2 {

using Tag = uint64_t;
static const Tag NullTag = 0LL;

enum class Race : uint8_t { Terran, Zerg, Protoss, Random };

inline auto RaceToString(const Race race) -> std::string {
    switch (race) {
        // case NoRace  : return "No Race";
        case Race::Terran:
            return "Terran";
        case Race::Zerg:
            return "Zerg";
        case Race::Protoss:
            return "Protoss";
        case Race::Random:
            return "Random";
        default:
            return "No Race";
    }
}

enum class GameResult : uint8_t { Win, Loss, Tie, Undecided };

enum class Difficulty : uint8_t {
    VeryEasy = 1,
    Easy = 2,
    Medium = 3,
    MediumHard = 4,
    Hard = 5,
    HardVeryHard = 6,
    VeryHard = 7,
    CheatVision = 8,
    CheatMoney = 9,
    CheatInsane = 10
};

inline auto DifficultyToString(const Difficulty difficulty) -> std::string {
    switch (difficulty) {
        case Difficulty::VeryEasy:
            return "Very Easy";
        case Difficulty::Easy:
            return "Easy";
        case Difficulty::Medium:
            return "Medium";
        case Difficulty::MediumHard:
            return "Medium Hard";
        case Difficulty::Hard:
            return "Hard";
        case Difficulty::HardVeryHard:
            return "Hard Very Hard";
        case Difficulty::VeryHard:
            return "Very Hard";
        case Difficulty::CheatVision:
            return "Cheat Vision";
        case Difficulty::CheatMoney:
            return "Cheat Money";
        case Difficulty::CheatInsane:
            return "Cheat Insane";
    }
    return "Is keyboard slamming a difficulty?";
}

enum class PlayerType : uint8_t { Participant = 1, Computer = 2, Observer = 3 };

enum class AIBuild : uint8_t { RandomBuild = 1, Rush = 2, Timing = 3, Power = 4, Macro = 5, Air = 6 };

inline auto AIBuildToString(const AIBuild build) -> std::string {
    switch (build) {
        case AIBuild::Rush:
            return "Rush";
        case AIBuild::Timing:
            return "Timing";
        case AIBuild::Power:
            return "Power";
        case AIBuild::Macro:
            return "Macro";
        case AIBuild::Air:
            return "Air";
        default:
            return "Random Build";
    }
}  // AIBuildToString

enum class ChatChannel : uint8_t { All = 0, Team = 1 };

class Agent;

//! Setup for a player in a game.
struct PlayerSetup {
    //! Player can be a Participant (usually an agent), Computer (in-built AI) or Observer.
    PlayerType type;
    //! Agent, if one is available.
    Agent* agent;
    //! Name of this player.
    std::string player_name;

    // Only used for Computer

    //! Race: Terran, Zerg or Protoss. Only for playing against the built-in AI.
    Race race;
    //! Difficulty: Only for playing against the built-in AI.
    Difficulty difficulty;
    //! Build type, used by computer opponent.
    AIBuild ai_build;

    PlayerSetup()
        : type(PlayerType::Participant),
          agent(nullptr),
          race(Race::Terran),
          difficulty(Difficulty::Easy),
          ai_build(AIBuild::RandomBuild) {};

    PlayerSetup(PlayerType in_type, Race in_race, Agent* in_agent = nullptr, std::string in_player_name = "",
                Difficulty in_difficulty = Difficulty::Easy, AIBuild in_ai_build = AIBuild::RandomBuild)
        : type(in_type),
          agent(in_agent),
          player_name(std::move(in_player_name)),
          race(in_race),
          difficulty(in_difficulty),
          ai_build(in_ai_build) {
    }
};

static inline PlayerSetup CreateParticipant(Race race, Agent* agent, const std::string& player_name = "") {
    return {PlayerType::Participant, race, agent, player_name};
}

static inline auto CreateComputer(Race race = Race::Random, Difficulty difficulty = Difficulty::Easy,
                                  AIBuild ai_build = AIBuild::RandomBuild, std::string computer_name = "")
    -> PlayerSetup {
    // Generates computer's name based on settings if no name is passed.
    if (computer_name.empty()) {
        computer_name =
            +"Computer-" + RaceToString(race) + "-" + DifficultyToString(difficulty) + "-" + AIBuildToString(ai_build);
    }
    return {PlayerType::Computer, race, nullptr, computer_name, difficulty, ai_build};
}

//! Port setup for a client.
struct PortSet {
    int game_port;
    int base_port;

    PortSet() : game_port(-1), base_port(-1) {
    }

    bool IsValid() const {
        return game_port > 0 && base_port > 0;
    }
};

//! Port setup for one or more clients in a game.
struct Ports {
    PortSet server_ports;
    std::vector<PortSet> client_ports;
    int shared_port;

    Ports() : shared_port(-1) {
    }

    bool IsValid() const {
        if (shared_port < 1)
            return false;
        if (!server_ports.IsValid())
            return false;
        if (client_ports.size() < 1)
            return false;
        for (std::size_t i = 0; i < client_ports.size(); ++i)
            if (!client_ports[i].IsValid())
                return false;

        return true;
    }
};

static const int max_path_size = 512;
static const int max_version_size = 32;
static const int max_num_players = 16;

//! Information about a player in a replay.
struct ReplayPlayerInfo {
    //! Player ID.
    int player_id;
    //! Display name from RequestReplayInfo. ResponseGameInfo during playback
    //! often leaves PlayerInfo.player_name empty; this is the reliable source.
    std::string name;
    //! Player ranking.
    int mmr;
    //! Player actions per minute.
    int apm;
    //! Actual player race.
    Race race;
    //! Selected player race. If the race is "Random", the race data member may be different.
    Race race_selected;
    //! If the player won or lost.
    GameResult game_result = GameResult::Undecided;

    ReplayPlayerInfo() : player_id(0), mmr(-10000), apm(0), race(Race::Random), race_selected(Race::Random) {
    }
};

//! Information about a replay file.
struct ReplayInfo {
    float duration;
    unsigned int duration_gameloops;
    int32_t num_players;
    uint32_t data_build;
    uint32_t base_build;
    std::string map_name;
    std::string map_path;
    std::string replay_path;
    std::string version;
    std::string data_version;
    ReplayPlayerInfo players[max_num_players];

    ReplayInfo() : duration(0.0f), duration_gameloops(0), num_players(0), data_build(0), base_build(0) {
    }

    bool GetPlayerInfo(ReplayPlayerInfo& replay_player_info, int playerID) const {
        for (int i = 0; i < num_players; ++i) {
            if (playerID == players[i].player_id) {
                replay_player_info = players[i];
                return true;
            }
        }

        return false;
    }

    float GetGameloopsPerSecond() const {
        return float(duration_gameloops) / duration;
    }
};

struct PlayerResult {
    PlayerResult(uint32_t player_id, GameResult result) : player_id(player_id), result(result) {};

    uint32_t player_id;
    GameResult result;
};

struct ChatMessage {
    uint32_t player_id;
    std::string message;
};

}  // namespace sc2
