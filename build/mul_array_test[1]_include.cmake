if(EXISTS "/Users/jeffwhynot/Documents/projects/Numcpp/build/mul_array_test")
  if(NOT EXISTS "/Users/jeffwhynot/Documents/projects/Numcpp/build/mul_array_test[1]_tests.cmake" OR
     NOT "/Users/jeffwhynot/Documents/projects/Numcpp/build/mul_array_test[1]_tests.cmake" IS_NEWER_THAN "/Users/jeffwhynot/Documents/projects/Numcpp/build/mul_array_test" OR
     NOT "/Users/jeffwhynot/Documents/projects/Numcpp/build/mul_array_test[1]_tests.cmake" IS_NEWER_THAN "${CMAKE_CURRENT_LIST_FILE}")
    include("/opt/homebrew/share/cmake/Modules/GoogleTestAddTests.cmake")
    gtest_discover_tests_impl(
      TEST_EXECUTABLE [==[/Users/jeffwhynot/Documents/projects/Numcpp/build/mul_array_test]==]
      TEST_EXECUTOR [==[]==]
      TEST_WORKING_DIR [==[/Users/jeffwhynot/Documents/projects/Numcpp/build]==]
      TEST_EXTRA_ARGS [==[]==]
      TEST_PROPERTIES [==[]==]
      TEST_PREFIX [==[]==]
      TEST_SUFFIX [==[]==]
      TEST_FILTER [==[]==]
      NO_PRETTY_TYPES [==[FALSE]==]
      NO_PRETTY_VALUES [==[FALSE]==]
      TEST_LIST [==[mul_array_test_TESTS]==]
      CTEST_FILE [==[/Users/jeffwhynot/Documents/projects/Numcpp/build/mul_array_test[1]_tests.cmake]==]
      TEST_DISCOVERY_TIMEOUT [==[5]==]
      TEST_DISCOVERY_EXTRA_ARGS [==[]==]
      TEST_XML_OUTPUT_DIR [==[]==]
    )
  endif()
  include("/Users/jeffwhynot/Documents/projects/Numcpp/build/mul_array_test[1]_tests.cmake")
else()
  add_test(mul_array_test_NOT_BUILT mul_array_test_NOT_BUILT)
endif()
