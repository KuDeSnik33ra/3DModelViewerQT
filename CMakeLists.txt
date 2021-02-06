cmake_minimum_required (VERSION 3.11)

project ("3DModelViewerQT")

find_package(Qt5 5.15 CONFIG REQUIRED COMPONENTS Core Gui Widgets)

set(CMAKE_MAP_IMPORTED_CONFIG_MINSIZEREL RelWithDebInfo Release)

if(CMAKE_CONFIGURATION_TYPES)
    set(CMAKE_CONFIGURATION_TYPES Debug Release RelWithDebInfo)
    set(CMAKE_CONFIGURATION_TYPES "${CMAKE_CONFIGURATION_TYPES}"
        CACHE STRING "Build Configurations" FORCE)
endif()

set(CMAKE_CXX_STANDARD 17)

add_subdirectory ("3DModelViewerQT")
