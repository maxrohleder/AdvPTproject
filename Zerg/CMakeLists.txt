

add_executable(mainZerg mainZerg.cpp)
if (EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/testZerg.cpp")
    add_executable(testZerg testZerg.cpp)
endif()