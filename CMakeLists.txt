cmake_minimum_required(VERSION 3.5)


project(simulation)

message("adding Zerg")
add_subdirectory(Zerg)
message("adding Terran")
add_subdirectory(Terran)
message("adding Protoss")
add_subdirectory(Protoss)

if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/Validator")
    message("adding Validator")
    add_subdirectory(Validator)
endif()

if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/RandomCreator")
    message("adding RandomCreator")
    add_subdirectory(RandomCreator)
endif()
