add_executable(
        tests
        tests.cpp
        ArrayTests.cpp
)

target_link_libraries(
        tests
        gtest_main
)

#add_test(NAME tests COMMAND tests)