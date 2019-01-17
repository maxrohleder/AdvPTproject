
add_executable(validatorTest validatorTest.cpp)

if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/validator_by_file")
    message("adding validator_by_file")
    add_subdirectory(validator_by_file)
endif()

add_executable(list_builder_test list_builder_test.cpp)