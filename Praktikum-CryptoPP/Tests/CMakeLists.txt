# Directories to look for lib files.
link_directories(
    ${CMAKE_LIBRARY_OUTPUT_DIRECTORY}
)

# List of all test files (one for each exercise).
set(AK_TEST_EXECUTABLES)
# List of all exercise libraries which expose the respective interface.
set(AK_TEST_LIBRARIES)
# List of the exercise headers.
set(AK_TEST_HEADERS)

list(APPEND AK_TEST_EXECUTABLES cryptopp-test.cpp)
list(APPEND AK_TEST_LIBRARIES cryptopp-intro-test) 
list(APPEND AK_TEST_HEADERS ./..)

# Directories to look for header files.
include_directories(
    ${CRYPTOPP_INCLUDE_DIR}
    ${AK_TEST_HEADERS} 
    # One doesn't need to include gtest/include but gmock... WHY???
    ${gmock_SOURCE_DIR}/include
)

add_executable(
    cryptopp_test
    # Add all test files here...
    ${AK_TEST_EXECUTABLES}
)

target_link_libraries(
    cryptopp_test
    gtest_main
    # Add all project libraries that expose crypto functions here...
    ${AK_TEST_LIBRARIES} 
)

include(GoogleTest)
gtest_discover_tests(cryptopp_test)


# Run Tests after each successful make build.
option(AK_AUTORUN_CRYPTOPP "Run Crypto++ tests after successful build" OFF)
if(AK_AUTORUN_CRYPTOPP)
add_custom_command(TARGET cryptopp_test COMMAND ../../bin/cryptopp_test POST_BUILD)
endif(AK_AUTORUN_CRYPTOPP)
