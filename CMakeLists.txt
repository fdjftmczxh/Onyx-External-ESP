cmake_minimum_required(VERSION 3.22.1)

project("onyx_external")

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(AIMGUI_BUILD_STATIC ON CACHE BOOL "" FORCE)
set(AIMGUI_BUILD_SHARED OFF CACHE BOOL "" FORCE)
set(AIMGUI_BUILD_TESTING OFF CACHE BOOL "" FORCE)

add_subdirectory(deps/AImGUI)
add_subdirectory(deps/KittyMemoryEx)

add_subdirectory(src/utils)
add_subdirectory(src/core)
add_subdirectory(src/render)

add_subdirectory(src)
