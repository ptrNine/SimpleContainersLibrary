cmake_minimum_required(VERSION 3.10)
project(SCL)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pedantic")

if(CMAKE_CXX_COMPILER_ID MATCHES GNU)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fprofile-arcs -ftest-coverage")
endif()

include_directories(${CMAKE_BINARY_DIR}/fakeroot/include)
link_directories(${CMAKE_BINARY_DIR}/fakeroot/lib)

include(CMake/git_submodule_build.cmake)
include(CMake/git_submodule_update.cmake)

git_submodule_update()
git_submodule_build(fmt CMAKE_ARGS -DFMT_TEST=OFF -DFMT_DOC=OFF -DFMT_INSTALL=ON)
git_submodule_build(utfcpp CMAKE_ARGS -DUTF8_SAMPLES=OFF -DUTF8_TESTS=OFF)

set(CMAKE_PREFIX_PATH ${CMAKE_BINARY_DIR}/fakeroot)

find_package(fmt REQUIRED)
find_package(utf8cpp REQUIRED)

set(SclIncludes
        scl/array.hpp
        scl/containers_base.hpp
        scl/math.hpp
        scl/ring.hpp
        scl/scl.hpp
        scl/string.hpp
        scl/traits.hpp
        scl/types.hpp
        scl/vector.hpp
        scl/vector2.hpp
        scl/vector3.hpp
)

set(SCL_INCLUDE_INSTALL_DIR  "${CMAKE_INSTALL_PREFIX}/include")
set(SCL_LIB_INSTALL_DIR      "${CMAKE_INSTALL_PREFIX}/lib")
set(SCL_TEST_INSTALL_DIR     "${CMAKE_INSTALL_PREFIX}/share/scl-tests")
set(SCL_CMAKE_INSTALL_DIR    "${SCL_LIB_INSTALL_DIR}/cmake/SCL")

option(SCL_BUILD_TESTS    "Build test programs" OFF)

install(
    FILES
        ${CMAKE_BINARY_DIR}/UseSCL.cmake
        ${CMAKE_BINARY_DIR}/SCLConfig.cmake
    DESTINATION
        ${SCL_CMAKE_INSTALL_DIR}
)

configure_file(
        ${CMAKE_SOURCE_DIR}/CMake/UseSCL.cmake.in
        ${CMAKE_BINARY_DIR}/UseSCL.cmake COPYONLY
)

configure_file(
        ${CMAKE_SOURCE_DIR}/CMake/SCLConfig.cmake.in
        ${CMAKE_BINARY_DIR}/SCLConfig.cmake @ONLY
)

install(
        FILES ${SclIncludes}
        DESTINATION ${SCL_INCLUDE_INSTALL_DIR}
)

if(SCL_BUILD_TESTS)
    add_subdirectory(tests)
endif()