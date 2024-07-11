# +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
# + CMakeLists.txt
# + Folder: src/Praktikum-CMake/Polybius
# +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

find_package(Gengetopt REQUIRED)

include_directories(.. ${CMAKE_SOURCE_DIR})

add_executable(polybius-test
    polybius-test.cpp
    PolybiusCipher.h PolybiusCipher.cpp
	Toolbox.h Toolbox.cpp)

