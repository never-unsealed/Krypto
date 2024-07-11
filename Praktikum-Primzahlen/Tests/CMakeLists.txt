# +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
# + CMakeLists.txt
# + Folder: src/Praktikum-Primzahlen/Tests
# +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

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

list(APPEND AK_TEST_EXECUTABLES primzahlen-test.cpp)
list(APPEND AK_TEST_LIBRARIES primes-test) 
list(APPEND AK_TEST_HEADERS ./..)

# Directories to look for header files.
include_directories(
    ${CRYPTOPP_INCLUDE_DIR}
    ${AK_TEST_HEADERS} 
    # One doesn't need to include gtest/include but gmock... WHY???
    ${gmock_SOURCE_DIR}/include
)

add_executable(
    primes-gtest
    # Add all test files here...
    ${AK_TEST_EXECUTABLES}
)

target_link_libraries(
    primes-gtest
    gtest_main
    # Add all project libraries that expose crypto functions here...
    ${AK_TEST_LIBRARIES} 
)

include(GoogleTest)
gtest_discover_tests(primes-gtest)


# Run Tests after each successful make build.
option(AK_AUTORUN_PRIMES "Run prime number tests after successful build" OFF)
if(AK_AUTORUN_PRIMES)
add_custom_command(TARGET primes_gtest COMMAND ../../bin/primes_test POST_BUILD)
endif(AK_AUTORUN_PRIMES)
