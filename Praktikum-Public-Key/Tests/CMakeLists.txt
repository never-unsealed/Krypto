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

list(APPEND AK_TEST_EXECUTABLES pubkey-test.cpp)
list(APPEND AK_TEST_LIBRARIES pubkey-test) 
list(APPEND AK_TEST_HEADERS ./..)

# Directories to look for header files.
include_directories(
    ${CRYPTOPP_INCLUDE_DIR}
    ${AK_TEST_HEADERS} 
    # One doesn't need to include gtest/include but gmock... WHY???
    ${gmock_SOURCE_DIR}/include
)

add_executable(
    pubkey-gtest
    # Add all test files here...
    ${AK_TEST_EXECUTABLES}
)

target_link_libraries(
    pubkey-gtest
    gtest_main
    # Add all project libraries that expose crypto functions here...
    ${AK_TEST_LIBRARIES} 
)

include(GoogleTest)
gtest_discover_tests(pubkey-gtest)


# Run Tests after each successful make build.
option(AK_AUTORUN_PUBKEY "Run public key tests after successful build" OFF)
if(AK_AUTORUN_PUBKEY)
add_custom_command(TARGET pubkey_test COMMAND ../../bin/pubkey_test POST_BUILD)
endif(AK_AUTORUN_PUBKEY)
