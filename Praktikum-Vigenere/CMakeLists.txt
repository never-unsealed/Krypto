# +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
# + CMakeLists.txt
# + Folder: src/Praktikum-Vigenere
# +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

find_package(Gengetopt REQUIRED)

add_custom_command(
        OUTPUT ${CMAKE_CURRENT_SOURCE_DIR}/kasiski-getopt.h
        ${CMAKE_CURRENT_SOURCE_DIR}/kasiski-getopt.c
        COMMAND ${GENGETOPT_EXECUTABLE} --include-getopt --unamed-opts
        -i kasiski-getopt.ggo -F kasiski-getopt
        DEPENDS kasiski-getopt.ggo
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
)

add_executable(
        kasiski kasiski-main.cpp
        VigenereBreaker.h VigenereBreaker.cpp
        VigenereCipher.h VigenereCipher.cpp
        ClassicCipher.h ClassicCipher.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/kasiski-getopt.h
        ${CMAKE_CURRENT_SOURCE_DIR}/kasiski-getopt.c
)

add_custom_command(
        OUTPUT ${CMAKE_CURRENT_SOURCE_DIR}/coindex-getopt.h
        ${CMAKE_CURRENT_SOURCE_DIR}/coindex-getopt.c
        COMMAND ${GENGETOPT_EXECUTABLE} --include-getopt --unamed-opts
        -i coindex-getopt.ggo -F coindex-getopt
        DEPENDS coindex-getopt.ggo
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
)

add_executable(
        coindex coindex-main.cpp
        VigenereBreaker.h VigenereBreaker.cpp
        VigenereCipher.h VigenereCipher.cpp
        ClassicCipher.h ClassicCipher.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/coindex-getopt.h
        ${CMAKE_CURRENT_SOURCE_DIR}/coindex-getopt.c
)

add_custom_command(
        OUTPUT ${CMAKE_CURRENT_SOURCE_DIR}/searchshift-getopt.h
        ${CMAKE_CURRENT_SOURCE_DIR}/searchshift-getopt.c
        COMMAND ${GENGETOPT_EXECUTABLE} --include-getopt --unamed-opts
        -i searchshift-getopt.ggo -F searchshift-getopt
        DEPENDS searchshift-getopt.ggo
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
)

add_executable(
        searchshift searchshift-main.cpp
        VigenereBreaker.h VigenereBreaker.cpp
        VigenereCipher.h VigenereCipher.cpp
        ClassicCipher.h ClassicCipher.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/searchshift-getopt.h
        ${CMAKE_CURRENT_SOURCE_DIR}/searchshift-getopt.c
)

add_custom_command(
        OUTPUT ${CMAKE_CURRENT_SOURCE_DIR}/vigenere-getopt.h
        ${CMAKE_CURRENT_SOURCE_DIR}/vigenere-getopt.c
        COMMAND ${GENGETOPT_EXECUTABLE} --include-getopt --unamed-opts
        -i vigenere-getopt.ggo -F vigenere-getopt
        DEPENDS vigenere-getopt.ggo
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
)

add_executable(
        vigenere vigenere-main.cpp
        VigenereBreaker.h VigenereBreaker.cpp
        VigenereCipher.h VigenereCipher.cpp
        ClassicCipher.h ClassicCipher.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/vigenere-getopt.h
        ${CMAKE_CURRENT_SOURCE_DIR}/vigenere-getopt.c
)

if(AK_COMPILE_TESTS)
    set(AK_Praktikum_Vigenere_Present ON PARENT_SCOPE) 

    ADD_LIBRARY(vigenere-test
            VigenereBreaker.h VigenereBreaker.cpp
            VigenereCipher.h VigenereCipher.cpp
            ClassicCipher.h ClassicCipher.cpp
    )

    add_subdirectory(Tests)
endif(AK_COMPILE_TESTS)

