if(EXISTS "/mnt/c/Users/never/Studium/6/Angewandte Kryptographie/repo/AK/src/Praktikum-Vigenere/Tests/vigenere-gtest[1]_tests.cmake")
  include("/mnt/c/Users/never/Studium/6/Angewandte Kryptographie/repo/AK/src/Praktikum-Vigenere/Tests/vigenere-gtest[1]_tests.cmake")
else()
  add_test(vigenere-gtest_NOT_BUILT vigenere-gtest_NOT_BUILT)
endif()
