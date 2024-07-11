# +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
# + CMakeLists.txt
# + Folder: src/Praktikum-DES
# +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

find_package(Gengetopt REQUIRED)

add_custom_command(
        OUTPUT ${CMAKE_CURRENT_SOURCE_DIR}/des-getopt.h
        ${CMAKE_CURRENT_SOURCE_DIR}/des-getopt.c
        COMMAND ${GENGETOPT_EXECUTABLE} --include-getopt --unamed-opts
        -i des-getopt.ggo -F des-getopt
        DEPENDS des-getopt.ggo
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
)

add_executable( des des-main.cpp
        DESCipher.h DESCipher.cpp
        BlockCipher.h BlockCipher.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/des-getopt.h
        ${CMAKE_CURRENT_SOURCE_DIR}/des-getopt.c
        )

# add_executable( desTest desTest DESCipher BlockCipher )

if(AK_COMPILE_TESTS)
    set(AK_Praktikum_DES_Present ON PARENT_SCOPE) 

    ADD_LIBRARY(des-test DESCipher.cpp BlockCipher.cpp)
    TARGET_LINK_LIBRARIES(des-test ${CRYPTOPP_LIBRARY} ${CMAKE_THREAD_LIBS_INIT})

    add_subdirectory(Tests)
endif(AK_COMPILE_TESTS)
