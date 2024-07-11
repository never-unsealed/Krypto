# +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
# + CMakeLists.txt
# + Folder: src/Praktikum-5
# +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

FIND_PACKAGE(CryptoPP REQUIRED)
FIND_PACKAGE(Threads REQUIRED)

INCLUDE_DIRECTORIES(${CRYPTOPP_INCLUDE_DIR})
LINK_DIRECTORIES(/usr/local/lib)

ADD_EXECUTABLE(cryptopp-intro cryptopp-intro.cpp
        PRNG.h PRNG.cpp
        BlumBlumShubGenerator.h BlumBlumShubGenerator.cpp)

TARGET_LINK_LIBRARIES(cryptopp-intro ${CRYPTOPP_LIBRARY} ${CMAKE_THREAD_LIBS_INIT})

if(AK_COMPILE_TESTS)
    set(AK_Praktikum_CryptoPP_Present ON PARENT_SCOPE) 

    ADD_LIBRARY(cryptopp-intro-test PRNG.cpp BlumBlumShubGenerator.cpp)
    TARGET_LINK_LIBRARIES(cryptopp-intro-test ${CRYPTOPP_LIBRARY} ${CMAKE_THREAD_LIBS_INIT})

    add_subdirectory(Tests)
endif(AK_COMPILE_TESTS)
