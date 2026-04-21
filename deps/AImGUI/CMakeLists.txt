cmake_minimum_required(VERSION 3.13.0)

project(AImGUI)

set(CMAKE_CXX_STANDARD 20)
add_compile_options(-fno-rtti -fvisibility=hidden)
add_link_options(-s)

find_library(liblog log NO_CACHE)

include_directories(
    modules
    includes
    third_party
    third_party/imgui
    ${CMAKE_CURRENT_SOURCE_DIR}/../KittyMemoryEx
)

# Scan imgui sources
aux_source_directory(third_party/imgui AIMGUI_IMGUI_SOURCES)

# Make imgui backends sources
set(
    AIMGUI_IMGUI_BACKENDS_SOURCES
    third_party/imgui/backends/imgui_impl_android.cpp
    third_party/imgui/backends/imgui_impl_opengl3.cpp
)

# Scan common sources
aux_source_directory(src/common AIMGUI_COMMON_SOURCES)

# Build AImGui library
add_library(AImGui STATIC 
    ${AIMGUI_COMMON_SOURCES} 
    ${AIMGUI_IMGUI_SOURCES} 
    ${AIMGUI_IMGUI_BACKENDS_SOURCES}
)

target_link_libraries(AImGui 
    ${liblog} 
    EGL 
    GLESv3 
    android
    KittyMemoryEx
)

target_include_directories(AImGui PUBLIC 
    modules
    includes
    third_party
    third_party/imgui
    ${CMAKE_CURRENT_SOURCE_DIR}/../KittyMemoryEx
)

set_target_properties(AImGui PROPERTIES
    ARCHIVE_OUTPUT_DIRECTORY "libs/${ANDROID_ABI}"
)