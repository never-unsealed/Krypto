if(EXISTS "/mnt/c/Users/never/Studium/6/Angewandte Kryptographie/repo/AK/src/Praktikum-Primzahlen/Tests/primes-gtest[1]_tests.cmake")
  include("/mnt/c/Users/never/Studium/6/Angewandte Kryptographie/repo/AK/src/Praktikum-Primzahlen/Tests/primes-gtest[1]_tests.cmake")
else()
  add_test(primes-gtest_NOT_BUILT primes-gtest_NOT_BUILT)
endif()
