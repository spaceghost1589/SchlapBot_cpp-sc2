#pragma once
#include <span>

#include "sc2api/sc2_agent.h"
#include "sc2api/sc2_coordinator.h"
#include "test_framework.h"

namespace sc2 {

bool TestUnitCommand(std::span<const char*> args);

}
