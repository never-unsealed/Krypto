if(EXISTS "/mnt/c/Users/never/Studium/6/Angewandte Kryptographie/repo/AK/src/Praktikum-AES/Tests/aes-gtest[1]_tests.cmake")
  include("/mnt/c/Users/never/Studium/6/Angewandte Kryptographie/repo/AK/src/Praktikum-AES/Tests/aes-gtest[1]_tests.cmake")
else()
  add_test(aes-gtest_NOT_BUILT aes-gtest_NOT_BUILT)
endif()