cmake_minimum_required(VERSION 3.20)

project(IntegralCalculatorApp LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 COMPONENTS Core Widgets Charts)
if(NOT Qt6_FOUND)
    find_package(Qt5 REQUIRED COMPONENTS Core Widgets Charts)
endif()

if(Qt6_FOUND)
    qt_standard_project_setup()
endif()

set(RELEASE_BUILD OFF)
set(DEBUG_INFO ON)

if (CMAKE_BUILD_TYPE STREQUAL "Release")
    set(RELEASE_BUILD ON)
    set(DEBUG_INFO OFF)
elseif(CMAKE_BUILD_TYPE STREQUAL "RelWithDebInfo")
    set(RELEASE_BUILD ON)
    set(DEBUG_INFO ON)
else()
    set(RELEASE_BUILD OFF)
    set(DEBUG_INFO ON)
endif()

if(RELEASE_BUILD)
    message("-- Build Type: Release")
else()
    message("-- Build Type: Debug")
endif()

if(DEBUG_INFO)
    message("-- Debug Info: Enabled")
else()
    message("-- Debug Info: Disabled")
endif()

if(DEBUG_INFO)
    add_compile_options("-ggdb3")
else()
    add_compile_options("-g0")
endif()

if(RELEASE_BUILD)
    add_compile_options("-O2")
endif()

file(GLOB_RECURSE SRC CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp" "${CMAKE_CURRENT_SOURCE_DIR}/src/*.hpp")

if(Qt6_FOUND)
    qt_add_executable(${PROJECT_NAME} ${SRC})
else()
    add_executable(${PROJECT_NAME} ${SRC})
endif()

if(Qt6_FOUND)
    target_link_libraries(${PROJECT_NAME} PRIVATE Qt6::Widgets Qt6::Charts)
else()
    target_link_libraries(${PROJECT_NAME} PRIVATE Qt5::Widgets Qt5::Charts)
endif()
