cmake_minimum_required(VERSION 3.15)

project(SchlapBot CXX)


# Define the executable using sources from the src/ directory
add_executable(SchlapBot
        main.cpp
        SchlapBot.cpp
        SchlapBot.h

)

file(GLOB_RECURSE LEGACY_FILES CONFIGURE_DEPENDS
        "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp"
        "${CMAKE_CURRENT_SOURCE_DIR}/src/*.h"
)
target_sources(SchlapBot PRIVATE ${LEGACY_FILES})

# Include directories for your bot and any local headers
target_include_directories(SchlapBot PRIVATE
        "${CMAKE_CURRENT_SOURCE_DIR}"       # For headers in the SchlapBot root
        "${CMAKE_CURRENT_SOURCE_DIR}/src"   # For headers in SchlapBot/src/
        "${CMAKE_SOURCE_DIR}/src"           # For cpp-sc2 headers
)

# Link directly to the existing cpp-sc2 library target
target_link_libraries(SchlapBot PRIVATE cpp_sc2)

#set_target_properties(SchlapBot PROPERTIES
#        CXX_STANDARD 23
#        CXX_STANDARD_REQUIRED ON
#        CXX_EXTENSIONS OFF
#)

## Set target properties (optional for VS, keeps things organized)
#set_target_properties(SchlapBot PROPERTIES
#        VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/src"
#)
