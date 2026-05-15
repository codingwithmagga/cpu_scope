add_executable(cpuscope_tests 
    test_perf_event.cpp
    test_cpu_profiler.cpp
    )

target_link_libraries(cpuscope_tests 
    PRIVATE 
        gtest_main 
        gmock_main 
        cpuscope_lib
    )

include(GoogleTest)
gtest_discover_tests(cpuscope_tests)
