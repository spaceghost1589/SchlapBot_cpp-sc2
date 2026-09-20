#include "sc2_unit_filters.h"

#include <algorithm>
#include <vector>

#include "sc2api/sc2_unit.h"
#include "sc2api/typeids/sc2_5.0.14_typeenums.h"
#include "sc2api/typeids/sc2_types.h"

namespace sc2 {

IsUnit::IsUnit(const UNIT_TYPEID type) : m_type(type) {
}

auto IsUnit::operator()(const Unit& unit) const -> bool {
    return unit.unit_type == m_type;
}

IsUnits::IsUnits(const std::vector<UNIT_TYPEID>& types) : m_types(types) {
}

auto IsUnits::operator()(const Unit& unit) const -> bool {
    for (const auto& type : m_types) {
        if (unit.unit_type == type) {
            return true;
        }
    }

    return false;
}

auto IsBasicTownHall::operator()(const Unit& unit_) const -> bool {
    return (*this)(unit_.unit_type);
}

auto IsBasicTownHall::operator()(const UNIT_TYPEID type_) const -> bool {
    return type_ == UNIT_TYPEID::PROTOSS_NEXUS || type_ == UNIT_TYPEID::TERRAN_COMMANDCENTER ||
           type_ == UNIT_TYPEID::ZERG_HATCHERY;
}

auto IsTownHall::operator()(const Unit& unit_) const -> bool {
    return (*this)(unit_.unit_type);
}

auto IsTownHall::operator()(const UNIT_TYPEID type_) const -> bool {
    return type_ == UNIT_TYPEID::TERRAN_COMMANDCENTERFLYING || type_ == UNIT_TYPEID::TERRAN_ORBITALCOMMAND ||
           type_ == UNIT_TYPEID::TERRAN_ORBITALCOMMANDFLYING || type_ == UNIT_TYPEID::TERRAN_PLANETARYFORTRESS ||
           type_ == UNIT_TYPEID::ZERG_HIVE || type_ == UNIT_TYPEID::ZERG_LAIR || IsBasicTownHall{}(type_);
}

auto IsMineralPatch::operator()(const Unit& unit) const -> bool {
    return (*this)(unit.unit_type);
}

auto IsMineralPatch::operator()(const UNIT_TYPEID type) const -> bool {
    return type == UNIT_TYPEID::NEUTRAL_BATTLESTATIONMINERALFIELD750 ||
           type == UNIT_TYPEID::NEUTRAL_BATTLESTATIONMINERALFIELD || type == UNIT_TYPEID::NEUTRAL_LABMINERALFIELD750 ||
           type == UNIT_TYPEID::NEUTRAL_LABMINERALFIELD || type == UNIT_TYPEID::NEUTRAL_MINERALFIELD750 ||
           type == UNIT_TYPEID::NEUTRAL_MINERALFIELD || type == UNIT_TYPEID::NEUTRAL_PURIFIERMINERALFIELD750 ||
           type == UNIT_TYPEID::NEUTRAL_PURIFIERMINERALFIELD ||
           type == UNIT_TYPEID::NEUTRAL_PURIFIERRICHMINERALFIELD750 ||
           type == UNIT_TYPEID::NEUTRAL_PURIFIERRICHMINERALFIELD || type == UNIT_TYPEID::NEUTRAL_RICHMINERALFIELD750 ||
           type == UNIT_TYPEID::NEUTRAL_RICHMINERALFIELD;
}

auto IsVisibleMineralPatch::operator()(const Unit& unit) const -> bool {
    return unit.mineral_contents > 0;
}

auto IsGeyser::operator()(const Unit& unit) const -> bool {
    return (*this)(unit.unit_type);
}

auto IsGeyser::operator()(const UNIT_TYPEID type) const -> bool {
    return type == UNIT_TYPEID::NEUTRAL_VESPENEGEYSER || type == UNIT_TYPEID::NEUTRAL_PROTOSSVESPENEGEYSER ||
           type == UNIT_TYPEID::NEUTRAL_SPACEPLATFORMGEYSER || type == UNIT_TYPEID::NEUTRAL_PURIFIERVESPENEGEYSER ||
           type == UNIT_TYPEID::NEUTRAL_SHAKURASVESPENEGEYSER || type == UNIT_TYPEID::NEUTRAL_RICHVESPENEGEYSER;
}

auto IsVisibleGeyser::operator()(const Unit& unit) const -> bool {
    return unit.vespene_contents > 0;
}

auto IsBuilding::operator()(const Unit& unit) const -> bool {
    return (*this)(unit.unit_type);
}

auto IsBuilding::operator()(const UNIT_TYPEID type) const -> bool {
    switch (type) {
        // Terran
        case UNIT_TYPEID::TERRAN_ARMORY:
        case UNIT_TYPEID::TERRAN_BARRACKS:
        case UNIT_TYPEID::TERRAN_BARRACKSFLYING:
        case UNIT_TYPEID::TERRAN_BARRACKSREACTOR:
        case UNIT_TYPEID::TERRAN_BARRACKSTECHLAB:
        case UNIT_TYPEID::TERRAN_BUNKER:
        case UNIT_TYPEID::TERRAN_COMMANDCENTER:
        case UNIT_TYPEID::TERRAN_COMMANDCENTERFLYING:
        case UNIT_TYPEID::TERRAN_ENGINEERINGBAY:
        case UNIT_TYPEID::TERRAN_FACTORY:
        case UNIT_TYPEID::TERRAN_FACTORYFLYING:
        case UNIT_TYPEID::TERRAN_FACTORYREACTOR:
        case UNIT_TYPEID::TERRAN_FACTORYTECHLAB:
        case UNIT_TYPEID::TERRAN_FUSIONCORE:
        case UNIT_TYPEID::TERRAN_GHOSTACADEMY:
        case UNIT_TYPEID::TERRAN_MISSILETURRET:
        case UNIT_TYPEID::TERRAN_ORBITALCOMMAND:
        case UNIT_TYPEID::TERRAN_ORBITALCOMMANDFLYING:
        case UNIT_TYPEID::TERRAN_PLANETARYFORTRESS:
        case UNIT_TYPEID::TERRAN_REFINERY:
        case UNIT_TYPEID::TERRAN_SENSORTOWER:
        case UNIT_TYPEID::TERRAN_STARPORT:
        case UNIT_TYPEID::TERRAN_STARPORTFLYING:
        case UNIT_TYPEID::TERRAN_STARPORTREACTOR:
        case UNIT_TYPEID::TERRAN_STARPORTTECHLAB:
        case UNIT_TYPEID::TERRAN_SUPPLYDEPOT:
        case UNIT_TYPEID::TERRAN_SUPPLYDEPOTLOWERED:
        case UNIT_TYPEID::TERRAN_REACTOR:
        case UNIT_TYPEID::TERRAN_TECHLAB:

        // Zerg
        case UNIT_TYPEID::ZERG_BANELINGNEST:
        case UNIT_TYPEID::ZERG_CREEPTUMOR:
        case UNIT_TYPEID::ZERG_CREEPTUMORBURROWED:
        case UNIT_TYPEID::ZERG_CREEPTUMORQUEEN:
        case UNIT_TYPEID::ZERG_EVOLUTIONCHAMBER:
        case UNIT_TYPEID::ZERG_EXTRACTOR:
        case UNIT_TYPEID::ZERG_GREATERSPIRE:
        case UNIT_TYPEID::ZERG_HATCHERY:
        case UNIT_TYPEID::ZERG_HIVE:
        case UNIT_TYPEID::ZERG_HYDRALISKDEN:
        case UNIT_TYPEID::ZERG_INFESTATIONPIT:
        case UNIT_TYPEID::ZERG_LAIR:
        case UNIT_TYPEID::ZERG_LURKERDENMP:
        case UNIT_TYPEID::ZERG_NYDUSCANAL:
        case UNIT_TYPEID::ZERG_NYDUSNETWORK:
        case UNIT_TYPEID::ZERG_ROACHWARREN:
        case UNIT_TYPEID::ZERG_SPAWNINGPOOL:
        case UNIT_TYPEID::ZERG_SPINECRAWLER:
        case UNIT_TYPEID::ZERG_SPINECRAWLERUPROOTED:
        case UNIT_TYPEID::ZERG_SPIRE:
        case UNIT_TYPEID::ZERG_SPORECRAWLER:
        case UNIT_TYPEID::ZERG_SPORECRAWLERUPROOTED:
        case UNIT_TYPEID::ZERG_ULTRALISKCAVERN:

        // Protoss
        case UNIT_TYPEID::PROTOSS_ASSIMILATOR:
        case UNIT_TYPEID::PROTOSS_CYBERNETICSCORE:
        case UNIT_TYPEID::PROTOSS_DARKSHRINE:
        case UNIT_TYPEID::PROTOSS_FLEETBEACON:
        case UNIT_TYPEID::PROTOSS_FORGE:
        case UNIT_TYPEID::PROTOSS_GATEWAY:
        case UNIT_TYPEID::PROTOSS_NEXUS:
        case UNIT_TYPEID::PROTOSS_PHOTONCANNON:
        case UNIT_TYPEID::PROTOSS_PYLON:
        case UNIT_TYPEID::PROTOSS_PYLONOVERCHARGED:
        case UNIT_TYPEID::PROTOSS_ROBOTICSBAY:
        case UNIT_TYPEID::PROTOSS_ROBOTICSFACILITY:
        case UNIT_TYPEID::PROTOSS_STARGATE:
        case UNIT_TYPEID::PROTOSS_TEMPLARARCHIVE:
        case UNIT_TYPEID::PROTOSS_TWILIGHTCOUNCIL:
        case UNIT_TYPEID::PROTOSS_WARPGATE:
        case UNIT_TYPEID::PROTOSS_SHIELDBATTERY:
            return true;

        default:
            return false;
    }
}

auto IsWorker::operator()(const Unit& unit_) const -> bool {
    return (*this)(unit_.unit_type);
}

auto IsWorker::operator()(const UNIT_TYPEID type_) const -> bool {
    return type_ == UNIT_TYPEID::TERRAN_SCV || type_ == UNIT_TYPEID::ZERG_DRONE || type_ == UNIT_TYPEID::PROTOSS_PROBE;
}

auto IsVisible::operator()(const Unit& unit_) const -> bool {
    return unit_.display_type == Unit::Visible;
};

auto IsCarryingMinerals(const Unit& unit) -> bool {
    const auto is_mineral = [](const BuffID& buff) -> bool {
        return buff == BUFF_ID::CARRYMINERALFIELDMINERALS || buff == BUFF_ID::CARRYHIGHYIELDMINERALFIELDMINERALS;
    };
    return std::ranges::find_if(unit.buffs, is_mineral) != unit.buffs.end();
}

auto IsCarryingVespene(const Unit& unit) -> bool {
    const auto is_vespene = [](const BuffID& buff) -> bool {
        return buff == BUFF_ID::CARRYHARVESTABLEVESPENEGEYSERGAS ||
               buff == BUFF_ID::CARRYHARVESTABLEVESPENEGEYSERGASPROTOSS ||
               buff == BUFF_ID::CARRYHARVESTABLEVESPENEGEYSERGASZERG;
    };
    return std::ranges::find_if(unit.buffs, is_vespene) != unit.buffs.end();
}

}  // namespace sc2
