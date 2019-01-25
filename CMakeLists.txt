cmake_minimum_required(VERSION 3.5)
# needed in Opt.h. hope it works for all
set(CMAKE_CXX_STANDARD 17)


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

if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/Optimization")
    message("adding Optimization")
    add_subdirectory(Optimization)
endif()
