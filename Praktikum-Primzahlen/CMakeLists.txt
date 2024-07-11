# +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
# + CMakeLists.txt
# + Folder: src/Praktikum-Primzahlen
# +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

INCLUDE_DIRECTORIES( ${CRYPTOPP_INCLUDE_DIR} ${CMAKE_CURRENT_SOURCE_DIR}/../Praktikum-CryptoPP)

LINK_DIRECTORIES( ${CRYPTOPP_LIBRARY_DIR} )


ADD_EXECUTABLE( primes primes-main.cpp
        FortunaGenerator.h FortunaGenerator.cpp
        MillerRabinTest.h MillerRabinTest.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/../Praktikum-CryptoPP/PRNG.h
        ${CMAKE_CURRENT_SOURCE_DIR}/../Praktikum-CryptoPP/PRNG.cpp
        )

TARGET_LINK_LIBRARIES(primes ${CRYPTOPP_LIBRARY} ${CMAKE_THREAD_LIBS_INIT})

if(AK_COMPILE_TESTS)
    set(AK_Praktikum_Primzahlen_Present ON PARENT_SCOPE) 

    ADD_LIBRARY(primes-test 
        FortunaGenerator.h FortunaGenerator.cpp
        MillerRabinTest.h MillerRabinTest.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/../Praktikum-CryptoPP/PRNG.h
        ${CMAKE_CURRENT_SOURCE_DIR}/../Praktikum-CryptoPP/PRNG.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/../Praktikum-CryptoPP/BlumBlumShubGenerator.h
        ${CMAKE_CURRENT_SOURCE_DIR}/../Praktikum-CryptoPP/BlumBlumShubGenerator.cpp
    )
    TARGET_LINK_LIBRARIES(primes-test ${CRYPTOPP_LIBRARY} ${CMAKE_THREAD_LIBS_INIT})

    add_subdirectory(Tests)
endif(AK_COMPILE_TESTS)
