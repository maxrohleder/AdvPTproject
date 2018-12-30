
add_executable(mainTerran mainTerran.cpp)
if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/testTerran.cpp")
    add_executable(testTerran testTerran.cpp)
endif()
