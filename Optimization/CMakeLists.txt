#add_executable(list_builder_test list_builder_test.cpp)
add_executable(mainOptimizer mainOptimizer.cpp)

if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/Opt_test.cpp")
    add_executable(Opt_test Opt_test.cpp)
endif()
