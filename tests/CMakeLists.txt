git_submodule_build(Catch2 CMAKE_ARGS -DCATCH_INSTALL_DOCS=OFF -DCATCH_BUILD_TESTING=OFF -DCATCH_INSTALL_HELPERS=ON)
find_package(Catch2 REQUIRED)
enable_testing()

add_executable(
        tests
        Main.cpp
        ArrayTests.cpp
        VectorTests.cpp
        StringTests.cpp
        RingTests.cpp
        MathVectorTests.cpp
        FormatTest.cpp
)

target_link_libraries(
        tests
        Catch2::Catch2
        fmt::fmt
)

include(CTest)
include(Catch)
catch_discover_tests(tests)