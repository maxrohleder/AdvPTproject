

add_executable(mainZerg mainZerg.cpp)
if (EXISTS "testZerg.cpp")
    add_executable(testZerg testZerg.cpp)
endif()