cmake_minimum_required(VERSION 3.16)

get_filename_component(source_dir "${CMAKE_CURRENT_LIST_DIR}/.." ABSOLUTE)
if(DEFINED CHESS_CHECK_BUILD_DIR)
    set(build_dir "${CHESS_CHECK_BUILD_DIR}")
else()
    set(build_dir "${source_dir}/build/environment-check")
endif()

execute_process(
    COMMAND "${CMAKE_COMMAND}" -S "${source_dir}" -B "${build_dir}" -DBUILD_TESTING=ON
    RESULT_VARIABLE configure_result)
if(NOT configure_result EQUAL 0)
    message(FATAL_ERROR "Environment check failed during CMake configuration")
endif()

execute_process(
    COMMAND "${CMAKE_COMMAND}" --build "${build_dir}"
    RESULT_VARIABLE build_result)
if(NOT build_result EQUAL 0)
    message(FATAL_ERROR "Environment check failed during compilation")
endif()

find_program(ctest_executable ctest)
if(NOT ctest_executable)
    message(FATAL_ERROR "Environment check requires CTest")
endif()
execute_process(
    COMMAND "${ctest_executable}" --output-on-failure
    WORKING_DIRECTORY "${build_dir}"
    RESULT_VARIABLE test_result)
if(NOT test_result EQUAL 0)
    message(FATAL_ERROR "Environment check failed during tests")
endif()

message(STATUS "Environment check passed: SFML found, assets present, project built and tested")
