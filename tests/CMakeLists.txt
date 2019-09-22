add_executable(
        tests
        tests.cpp
        ArrayTests.cpp
        VectorTests.cpp
        StringTests.cpp
)

target_link_libraries(
        tests
        gtest
        gtest_main
        fmt
)

add_test(NAME tests COMMAND tests)