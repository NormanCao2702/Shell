if(EXISTS "/Users/tran/Desktop/Shell/build/shell_test[1]_tests.cmake")
  include("/Users/tran/Desktop/Shell/build/shell_test[1]_tests.cmake")
else()
  add_test(shell_test_NOT_BUILT shell_test_NOT_BUILT)
endif()