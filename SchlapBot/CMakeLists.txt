cmake_minimum_required(VERSION 3.15)

project(SchlapBot CXX)

# Define the executable using sources from the src/ directory
add_executable(SchlapBot
        main.cpp
        SchlapBot.cpp
)

file(GLOB_RECURSE FILES CONFIGURE_DEPENDS
        "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp"
        "${CMAKE_CURRENT_SOURCE_DIR}/*.h"
)

target_sources(SchlapBot PRIVATE ${FILES})
# Include directories for your bot and any local headers
target_include_directories(SchlapBot PRIVATE
        src
)

# Link directly to the existing cpp-sc2 library target
target_link_libraries(SchlapBot PRIVATE cpp_sc2)

# Set target properties (optional for VS, keeps things organized)
set_target_properties(SchlapBot PROPERTIES
        VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/src"
)
