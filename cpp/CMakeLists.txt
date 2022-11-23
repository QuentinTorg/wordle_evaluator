
cmake_minimum_required(VERSION 3.0)
project(production_calibration VERSION 0.0.0.0 LANGUAGES C CXX)

set(BUILD_SHARED_LIBS ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_compile_options(-Wall
                    -Wextra
                    -Werror
                    -Wfatal-errors
                    -Wpedantic
                    -Wno-variadic-macros)

if (CMAKE_CXX_COMPILER MATCHES "Clang")
    add_compile_options(-Wno-variadic-macros
                        -Wno-embedded-directive
                        -Wno-zero-length-array
                        -Wno-gnu-zero-variadic-macro-arguments)
endif()

if (NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
    message(STATUS "Selecting default build type: Release")
    set(CMAKE_BUILD_TYPE "Release" CACHE STRING "Build type (default: Release)" FORCE)
endif ()

option(BUILD_TESTING "Build unit tests." ON)

if (BUILD_TESTING)
    find_package(GTest REQUIRED)
    enable_testing()
else()
    message("Skipping tests build")
endif()



add_library(wordle_evaluator SHARED
    src/evaluator.cc
    src/utils.cc
    )

target_include_directories(wordle_evaluator
        PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        PUBLIC $<INSTALL_INTERFACE:include>)

    #add_executable(wordle_test src/wordle_test.cc)
    #target_link_libraries(wordle_test
    #    PRIVATE
    #        wordle_evaluator)

if (BUILD_TESTING AND GTEST_FOUND)
    add_subdirectory(test)
endif()
