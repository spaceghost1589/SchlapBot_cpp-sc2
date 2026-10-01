#pragma once

#if defined(__GNUC__) || defined(__clang__)
#pragma GCC warning "Header sc2_lib.h is deprecated. Please use specific headers instead."
#elif defined(_MSC_VER)
#pragma message("WARNING: Header sc2_lib.h is deprecated. Please use specific headers instead.")
#endif

#include "sc2_search.h"
#include "sc2_utils.h"
