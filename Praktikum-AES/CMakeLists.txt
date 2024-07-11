# +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
# + CMakeLists.txt
# + Folder: src/Praktikum-AES
# +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++


#set(CMAKE_CXX_STANDARD 11)

add_executable(aestests aestests-main.cpp AESMath.cpp AESMath.h datatypes.h
        AESState.cpp AESState.h
        AESKeySchedule.cpp AESKeySchedule.h
        AESCipher.cpp AESCipher.h
        BlockCipher.h BlockCipher.cpp)

add_custom_command(
        OUTPUT ${CMAKE_CURRENT_SOURCE_DIR}/aes-getopt.h
        ${CMAKE_CURRENT_SOURCE_DIR}/aes-getopt.c
        COMMAND ${GENGETOPT_EXECUTABLE} --include-getopt --unamed-opts -i aes-getopt.ggo -F aes-getopt
        DEPENDS aes-getopt.ggo
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
)

add_executable(aes aes-main.cpp
        datatypes.h
        AESMath.h AESMath.cpp
        AESKeySchedule.h AESKeySchedule.cpp
        BlockCipher.h BlockCipher.cpp
        AESCipher.h AESCipher.cpp
        AESState.h AESState.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/aes-getopt.h
        ${CMAKE_CURRENT_SOURCE_DIR}/aes-getopt.c
        )

if(AK_COMPILE_TESTS)
    set(AK_Praktikum_AES_Present ON PARENT_SCOPE) 

    ADD_LIBRARY(aes-test 
        datatypes.h
        AESMath.h AESMath.cpp
        AESKeySchedule.h AESKeySchedule.cpp
        BlockCipher.h BlockCipher.cpp
        AESCipher.h AESCipher.cpp
        AESState.h AESState.cpp
    )
    TARGET_LINK_LIBRARIES(aes-test ${CRYPTOPP_LIBRARY} ${CMAKE_THREAD_LIBS_INIT})

    add_subdirectory(Tests)
endif(AK_COMPILE_TESTS)
