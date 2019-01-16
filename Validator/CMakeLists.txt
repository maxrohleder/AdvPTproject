
add_executable(validatorTest validatorTest.cpp)

if(EXISTS "validator_by_file")
    add_subdirectory(validator_by_file)
endif()