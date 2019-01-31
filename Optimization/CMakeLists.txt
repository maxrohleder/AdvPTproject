#add_executable(list_builder_test list_builder_test.cpp)
add_executable(mainOptimizer mainOptimizer.cpp)

set(CMAKE_BUILD_TYPE Debug)

if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/Opt_test.cpp")
    add_executable(Opt_test Opt_test.cpp)
endif()

if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/Opt_test.cpp")
    add_executable(popt protoss_opt.cpp)
endif()
