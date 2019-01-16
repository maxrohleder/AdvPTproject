cmake_minimum_required(VERSION 3.5)


project(simulation)

add_subdirectory(Zerg)
add_subdirectory(Terran)
add_subdirectory(Protoss)

if(EXISTS "Validator")
    add_subdirectory(Validator)
endif()

if(EXISTS "RandomCreator")
    add_subdirectory(RandomCreator)
endif()
