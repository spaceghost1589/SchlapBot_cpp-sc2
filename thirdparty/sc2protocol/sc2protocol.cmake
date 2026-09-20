message(STATUS "FetchContent: protocol")

FetchContent_Declare(
    sc2protocol
    GIT_REPOSITORY https://github.com/Blizzard/s2client-proto.git
    GIT_TAG bff45dae1fc685e6acbaae084670afb7d1c0832c
    GIT_PROGRESS TRUE
    UPDATE_DISCONNECTED TRUE
    PATCH_COMMAND git apply --ignore-space-change --ignore-whitespace "${CMAKE_CURRENT_LIST_DIR}/proto3.patch"
)
FetchContent_MakeAvailable(sc2protocol)
