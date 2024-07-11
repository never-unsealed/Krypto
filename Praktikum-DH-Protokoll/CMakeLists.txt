# +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
# + CMakeLists.txt
# + Folder: src/Praktikum-DH-Protokoll
# +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

FIND_PACKAGE(CryptoPP REQUIRED)
FIND_PACKAGE(Threads REQUIRED)
FIND_PACKAGE(Asio REQUIRED)

INCLUDE_DIRECTORIES( ${CRYPTOPP_INCLUDE_DIR} ${ASIO_INCLUDE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/../Praktikum-CryptoPP
    ${CMAKE_CURRENT_SOURCE_DIR}/../Praktikum-Primzahlen )

LINK_DIRECTORIES( ${CRYPTOPP_LIBRARY_DIR} )

ADD_EXECUTABLE( enctest enctest-main.cpp MessageEncoder.h MessageEncoder.cpp )
TARGET_LINK_LIBRARIES(enctest ${CRYPTOPP_LIBRARY} ${CMAKE_THREAD_LIBS_INIT})

ADD_EXECUTABLE( netintro netintro-main.cpp netintro.h netintro.cpp MessageEncoder.h MessageEncoder.cpp )
TARGET_LINK_LIBRARIES(netintro ${CRYPTOPP_LIBRARY} ${CMAKE_THREAD_LIBS_INIT})

ADD_EXECUTABLE( dhprotocol
        dhprotocol-main.cpp
        MessageEncoder.h MessageEncoder.cpp
        DHProtocol.h DHProtocol.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/../Praktikum-CryptoPP/PRNG.h
        ${CMAKE_CURRENT_SOURCE_DIR}/../Praktikum-CryptoPP/PRNG.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/../Praktikum-Primzahlen/FortunaGenerator.h
        ${CMAKE_CURRENT_SOURCE_DIR}/../Praktikum-Primzahlen/FortunaGenerator.cpp
        )
TARGET_LINK_LIBRARIES(dhprotocol ${CRYPTOPP_LIBRARY} ${CMAKE_THREAD_LIBS_INIT})

if(AK_COMPILE_TESTS)
    set(AK_Praktikum_DH_Present ON PARENT_SCOPE) 

    ADD_LIBRARY(dh-test 
        MessageEncoder.h MessageEncoder.cpp
        DHProtocol.h DHProtocol.cpp
        netintro.h netintro.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/../Praktikum-CryptoPP/PRNG.h
        ${CMAKE_CURRENT_SOURCE_DIR}/../Praktikum-CryptoPP/PRNG.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/../Praktikum-Primzahlen/FortunaGenerator.h
        ${CMAKE_CURRENT_SOURCE_DIR}/../Praktikum-Primzahlen/FortunaGenerator.cpp
    )
    TARGET_LINK_LIBRARIES(dh-test ${CRYPTOPP_LIBRARY} ${CMAKE_THREAD_LIBS_INIT})

    add_subdirectory(Tests)
endif(AK_COMPILE_TESTS)
