cmake_minimum_required(VERSION 3.20)

project(IntegralCalculatorApp LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 REQUIRED COMPONENTS Core Widgets)

qt_standard_project_setup()

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
    message("Build Type: Release")
else()
    message("Build Type: Debug")
endif()

if(DEBUG_INFO)
    message("Debug Info: Enabled")
else()
    message("Debug Info: Disabled")
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

qt_add_executable(${PROJECT_NAME} ${SRC})

target_link_libraries(${PROJECT_NAME} PRIVATE Qt6::Widgets)
