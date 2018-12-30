cmake_minimum_required(VERSION 3.8)


project(simulation)

add_subdirectory(Zerg)
add_subdirectory(Terran)
add_subdirectory(Protoss)
if(EXISTS "Protoss/validator")
    add_subdirectory(Protoss/validator)
endif()
if(EXISTS "Validator")
    add_subdirectory(Validator)
endif()
