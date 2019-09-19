cmake_minimum_required(VERSION 3.13)
project(SCL)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pedantic")

if(CMAKE_CXX_COMPILER_ID MATCHES GNU)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fprofile-arcs -ftest-coverage")
endif()

find_package(fmt REQUIRED)

include_directories(3p/usr/local/include) # TODO: delete this later
link_directories(3p/usr/local/lib)        # TODO: delete this later

set(SclIncludes
        scl/array.hpp
        scl/containers_base.hpp
        scl/cp_string.hpp
        scl/traits.hpp
        scl/ring.hpp
        scl/string.hpp
        scl/vector.hpp
        scl/vector2.hpp
        scl/vector3.hpp
)

set(SCL_INCLUDE_INSTALL_DIR "${CMAKE_INSTALL_PREFIX}/include")
set(SCL_EXAMPLES_INSTALL_DIR "${CMAKE_INSTALL_PREFIX}/share/scl-examples")
set(SCL_LIB_INSTALL_DIR "${CMAKE_INSTALL_PREFIX}/lib")
set(SCL_TEST_INSTALL_DIR "${CMAKE_INSTALL_PREFIX}/share/scl-tests")
set(SCL_CMAKE_INSTALL_DIR "${SCM_LIB_INSTALL_DIR}/cmake/SCL")

option(SCM_BUILD_TESTS "Build test programs" OFF)
option(SCM_BUILD_EXAMPLES "Build example programs" OFF)

install(
    FILES
        ${PROJECT_BINARY_DIR}/UseSCL.cmake
        ${PROJECT_BINARY_DIR}/SCLConfig.cmake
    DESTINATION
        ${SCL_CMAKE_INSTALL_DIR}
)

configure_file(
        ${PROJECT_SOURCE_DIR}/CMake/UseSCL.cmake.in
        ${PROJECT_BINARY_DIR}/UseSCL.cmake COPYONLY
)

configure_file(
        ${PROJECT_SOURCE_DIR}/CMake/SCLConfig.cmake.in
        ${PROJECT_BINARY_DIR}/SCLConfig.cmake @ONLY
)

install(
        FILES ${SclIncludes}
        DESTINATION ${SCL_INCLUDE_INSTALL_DIR}
)

if(SCL_BUILD_TESTS)
    include(CMake/fetch_googletest.cmake)
    fetch_googletest(
            ${PROJECT_SOURCE_DIR}/CMake
            ${PROJECT_BINARY_DIR}/googletest
    )
    enable_testing()
    add_subdirectory(tests)
endif()

if(SCL_BUILD_EXAMPLES)
    #add_subdirectory(examples)
endif()


add_executable(SCL main.cpp)
target_link_libraries(SCL fmt)