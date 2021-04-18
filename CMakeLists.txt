cmake_minimum_required(VERSION 3.14)

set(PROJECT "MyCppToolingAndStuff")

project("${PROJECT}_Project" VERSION 1.2.3 LANGUAGES CXX)

# Global CMake variables are set here
set(CMAKE_CXX_STANDARD          17) #20)# hat er irgendwie noch nicht
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS        OFF)

#Options
option(ENABLE_TESTING   "Enable a Unit Testing build."  ON)
option(ENABLE_COVERAGE  "Enable Code Coverage build."   OFF)
option(ENABLE_BENCHMARK "Enable Code Coverage build."   ON)

# Cmake modules
set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake/)
#build target to build docu is: make MyCppToolingAndStuff_docs 
include(Documentation)
if (ENABLE_TESTING OR ENABLE_COVERAGE)
    include(CTest)
endif()
if (ENABLE_COVERAGE)
    if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
        include(CodeCoverage)
    else()
        message("Coverage only for GNU|Clang")
        set(ENABLE_COVERAGE OFF)
    endif()
endif()



# Project/Library Names
set(LIBRARY_NAME    "${PROJECT}_MyLib")
set(MAIN            "${PROJECT}_Executable")
set(TEST_MAIN       "${PROJECT}_unit_tests")
set(COVERAGE_MAIN   "${PROJECT}_coverage")
set(BENCHMARK_MAIN  "${PROJECT}_benchmarks")

find_package(Boost COMPONENTS system filesystem program_options log regex serialization REQUIRED)
include_directories(${Boost_INCLUDE_DIR})
link_directories(${Boost_LIBRARY_DIR})
add_definitions (-DBOOST_ALL_DYN_LINK)


# Add conan libraries
include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup()

# Add external libraries
add_subdirectory("${PROJECT_SOURCE_DIR}/external/")

# Add first-party libraries
add_subdirectory("${PROJECT_SOURCE_DIR}/src/")

# Add executables
add_subdirectory("${PROJECT_SOURCE_DIR}/app/")

if (ENABLE_TESTING OR ENABLE_COVERAGE)
enable_testing() #activates CTest packet
    add_subdirectory("${PROJECT_SOURCE_DIR}/tests/")
endif()


# Add benchmarks
if(ENABLE_BENCHMARK)
    add_subdirectory("${PROJECT_SOURCE_DIR}/benchmarks/")
endif()
