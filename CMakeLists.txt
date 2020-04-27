cmake_minimum_required(VERSION 3.12)
project(GameOfLife VERSION 0.0.1 LANGUAGES CXX)

find_program(CCACHE ccache)
if(CCACHE)
  set(CMAKE_CXX_COMPILER_LAUNCHER ${CCACHE})
endif()

# Set a default build type if none was specified
if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
    message(STATUS "Setting build type to 'Release' as none was specified.")
    set(CMAKE_BUILD_TYPE Release CACHE STRING "Choose the type of build." FORCE)

    # Set the possible values of build type for cmake-gui
    set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS
        "Debug"
        "Release"
        "MinSizeRel"
        "RelWithDebInfo"
    )
endif()

# Link this 'library' to use the standard warnings
add_library(project_warnings INTERFACE)

# Link this 'library' to set the c++ standard / compile-time options requested
add_library(project_options INTERFACE)

target_compile_features(project_options INTERFACE cxx_std_17)
target_include_directories(project_options INTERFACE include)

target_compile_options(project_warnings INTERFACE
    -Wall
    -Wextra
    -Wpedantic
    -Wshadow
    -Wold-style-cast
    -Wcast-align
    -Wnon-virtual-dtor
    -Woverloaded-virtual
)


option(ENABLE_CLANG_TIDY "Enable testing with clang-tidy" OFF)
option(ENABLE_CPPCHECK "Enable testing with cppcheck" OFF)

if(ENABLE_CLANG_TIDY)
    find_program(CLANGTIDY clang-tidy)
    if(CLANGTIDY)
        set(CMAKE_CXX_CLANG_TIDY ${CLANGTIDY})
    else()
        message(SEND_ERROR "clang-tidy requested but executable not found")
    endif()
endif()

if(ENABLE_CPPCHECK)
    find_program(CPPCHECK cppcheck)
    if(CPPCHECK)
        set(CMAKE_CXX_CPPCHECK ${CPPCHECK}
                --enable=warning,performance
                --inconclusive
                --force
                --inline-suppr)
    else()
        message(SEND_ERROR "cppcheck requested but executable not found")
    endif()
endif()

# Download automatically, you can also just copy the conan.cmake file
if(NOT EXISTS "${CMAKE_BINARY_DIR}/conan.cmake")
   message(STATUS "Downloading conan.cmake from https://github.com/conan-io/cmake-conan")
   file(DOWNLOAD "https://github.com/conan-io/cmake-conan/raw/v0.15/conan.cmake"
                 "${CMAKE_BINARY_DIR}/conan.cmake")
endif()

include(${CMAKE_BINARY_DIR}/conan.cmake)
conan_check(REQUIRED)
conan_cmake_run(
    CONANFILE conanfile.txt
    BASIC_SETUP
    CMAKE_TARGETS
    BUILD missing
)

set(CMAKE_MODULE_PATH "/usr/share/SFML/cmake/Modules/" ${CMAKE_MODULE_PATH})
find_package(SFML COMPONENTS system window graphics REQUIRED)

add_executable(${PROJECT_NAME})

target_link_libraries(${PROJECT_NAME} PRIVATE
    project_options
    project_warnings
    ${SFML_LIBRARIES}
    boost_program_options
    CONAN_PKG::spdlog
)

target_sources(${PROJECT_NAME}
    PUBLIC
        ${PROJECT_SOURCE_DIR}/include/Grid.hpp
        ${PROJECT_SOURCE_DIR}/include/DrawableGrid.hpp
    PRIVATE
        ${PROJECT_SOURCE_DIR}/src/Grid.cpp
        ${PROJECT_SOURCE_DIR}/src/DrawableGrid.cpp
        ${PROJECT_SOURCE_DIR}/src/GameOfLife.cpp
)

# Testing
option(BUILD_TESTS "Tells CMake to build all the tests" OFF)

if(BUILD_TESTS)
    enable_testing()
    add_subdirectory(tests)
endif()
