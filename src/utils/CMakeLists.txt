add_library(onyx_utils INTERFACE)

target_include_directories(onyx_utils INTERFACE 
    ${PROJECT_SOURCE_DIR}/include
    ${PROJECT_SOURCE_DIR}/deps/AImGUI/third_party/imgui
    ${PROJECT_SOURCE_DIR}/deps/AImGUI/third_party/imgui/backends
    ${PROJECT_SOURCE_DIR}/deps/AImGUI/includes
)

target_link_libraries(onyx_utils INTERFACE
    KittyMemoryEx
    AImGui
)
