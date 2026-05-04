add_executable(cpuscope_tests test_dummy.cpp)

target_link_libraries(cpuscope_tests PRIVATE gtest_main cpuscope_lib)

include(GoogleTest)
gtest_discover_tests(cpuscope_tests)
