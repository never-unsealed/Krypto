# +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
# + CMakeLists.txt
# + Folder: src/Praktikum-Public-Key
# +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

FIND_PACKAGE(CryptoPP REQUIRED)
FIND_PACKAGE(Threads REQUIRED)

INCLUDE_DIRECTORIES(${CRYPTOPP_INCLUDE_DIR})
ADD_EXECUTABLE(pubkey pubkey-main.cpp
        PublicKeyAlgorithmBox.h PublicKeyAlgorithmBox.cpp)
TARGET_LINK_LIBRARIES(pubkey ${CRYPTOPP_LIBRARY} ${CMAKE_THREAD_LIBS_INIT})

ADD_EXECUTABLE(rabin rabin-main.cpp
        RabinEncryptor.h RabinEncryptor.cpp
        RabinDecryptor.h RabinDecryptor.cpp
        RabinAttack.h RabinAttack.cpp
        PublicKeyAlgorithmBox.h PublicKeyAlgorithmBox.cpp)
TARGET_LINK_LIBRARIES(rabin ${CRYPTOPP_LIBRARY} ${CMAKE_THREAD_LIBS_INIT})

ADD_EXECUTABLE(rsa rsa-main.cpp
        PublicKeyAlgorithmBox.h PublicKeyAlgorithmBox.cpp
        RSAOracle.h RSAOracle.cpp
        RSAEncryptor.h RSAEncryptor.cpp
        RSADecryptor.h RSADecryptor.cpp
        RSAAttack.h RSAAttack.cpp)
TARGET_LINK_LIBRARIES(rsa ${CRYPTOPP_LIBRARY} ${CMAKE_THREAD_LIBS_INIT})

ADD_EXECUTABLE(rsaattack rsaattack-main.cpp
        PublicKeyAlgorithmBox.h PublicKeyAlgorithmBox.cpp
        RSAOracle.h RSAOracle.cpp
        RSAEncryptor.h RSAEncryptor.cpp
        RSADecryptor.h RSADecryptor.cpp
        RSAAttack.h RSAAttack.cpp)
TARGET_LINK_LIBRARIES(rsaattack ${CRYPTOPP_LIBRARY} ${CMAKE_THREAD_LIBS_INIT})

if(AK_COMPILE_TESTS)
    set(AK_Praktikum_Public_Key_Present ON PARENT_SCOPE) 

    ADD_LIBRARY(pubkey-test
        RabinEncryptor.h RabinEncryptor.cpp
        RabinDecryptor.h RabinDecryptor.cpp
        RabinAttack.h RabinAttack.cpp
        PublicKeyAlgorithmBox.h PublicKeyAlgorithmBox.cpp
        RSAOracle.h RSAOracle.cpp
        RSAEncryptor.h RSAEncryptor.cpp
        RSADecryptor.h RSADecryptor.cpp
        RSAAttack.h RSAAttack.cpp
    )
    TARGET_LINK_LIBRARIES(pubkey-test ${CRYPTOPP_LIBRARY} ${CMAKE_THREAD_LIBS_INIT})

    add_subdirectory(Tests)
endif(AK_COMPILE_TESTS)
