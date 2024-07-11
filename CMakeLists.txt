# +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
# + CMakeLists.txt
# + Folder: src
# +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

Project(AngewandteKryptographie)

cmake_minimum_required(VERSION 3.10)
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
#set(CMAKE_VERBOSE_MAKEFILE true)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/bin )
set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/CMakeModules )
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/lib )

# Test build instruction.
option(AK_COMPILE_TESTS "Compile all unit tests" ON)

#################################
# Testing
#################################

if(AK_COMPILE_TESTS)
    # Deklare a dependency on GoogleTest which is downloaded from GitHub automatically.
    include(FetchContent)
    FetchContent_Declare(
      googletest
      # This commit hash should be updated regularly.
      URL https://github.com/google/googletest/archive/bf66935e07825318ae519675d73d0f3e313b3ec6.zip
    )
    # For Windows: Prevent overriding the parent project's compiler/linker settings
    set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
    FetchContent_MakeAvailable(googletest)
endif(AK_COMPILE_TESTS)

#################################
# Project
#################################

#set (UNITTEST_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/unit_tests )
#set(CTEST_BINARY_DIRECTORY ${UNITTEST_OUTPUT_DIRECTORY})



# set(CMAKE_CXX_FLAGS "-std=c++0x")


# Prepare "Catch" library for other executables
#set(CATCH_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/Catch/include)
#add_library(Catch INTERFACE)
#target_include_directories(Catch INTERFACE ${CATCH_INCLUDE_DIR})

#enable_testing()

add_subdirectory(Praktikum-AES)
add_subdirectory(Praktikum-CMake)
add_subdirectory(Praktikum-CryptoPP)
add_subdirectory(Praktikum-DES)
add_subdirectory(Praktikum-Vigenere)
add_subdirectory(Praktikum-Primzahlen)
add_subdirectory(Praktikum-DH-Protokoll)
add_subdirectory(Praktikum-Public-Key)

#################################
# Testing
#################################

if(AK_COMPILE_TESTS)
    # The Tests are located within each project subdir
    enable_testing()
endif(AK_COMPILE_TESTS)

#################################
# Documentation
#################################

option(GENERATE_DOCS "Generate documentation for project" TRUE)
if(GENERATE_DOCS)
    add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/Doxygen)
endif(GENERATE_DOCS)

SET( CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH}
${PROJECT_SOURCE_DIR}/CMakeModules)
