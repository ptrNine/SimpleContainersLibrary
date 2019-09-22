macro(git_submodule_build _project_name)
    set(options "")
    set(oneValueArgs CMAKE_ARGS)
    set(multiValueArgs "")
    cmake_parse_arguments(${_project_name} "${options}" "${oneValueArgs}" "${multiValueArgs}" ${ARGN})

    project(${_project_name}_download)

    message("-- Submodule '${_project_name}' at ${PROJECT_SOURCE_DIR}/remote/${_project_name}")
    message("    BUILD_TYPE: ${CMAKE_BUILD_TYPE}")
    message("    C_COMPILER: ${CMAKE_C_COMPILER}")
    message("    CXX_COMPILER: ${CMAKE_CXX_COMPILER}")
    message("    INSTALL_PREFIX: ${PROJECT_BINARY_DIR}/fakeroot")

    execute_process(COMMAND ${CMAKE_COMMAND} -G "${CMAKE_GENERATOR}" .
            -DCMAKE_CXX_COMPILER=${CMAKE_CXX_COMPILER}
            -DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE}
            -DCMAKE_INSTALL_PREFIX=${PROJECT_BINARY_DIR}/fakeroot
            -B${PROJECT_BINARY_DIR}/remote/${_project_name}
            ${${_project_name}_CMAKE_ARGS}
            RESULT_VARIABLE result
            WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}/remote/${_project_name}
    )

    if(result)
        message(FATAL_ERROR "CMake step for ${_project_name} failed: ${result}")
    endif()

    execute_process(COMMAND ${CMAKE_COMMAND} --build . --target install
            RESULT_VARIABLE result
            WORKING_DIRECTORY ${PROJECT_BINARY_DIR}/remote/${_project_name}
    )

    if(result)
        message(FATAL_ERROR "Build step for ${_project_name} failed: ${result}")
    endif()

endmacro()