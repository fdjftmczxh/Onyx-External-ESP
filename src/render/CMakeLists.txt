add_library(onyx_render STATIC
    renderer.cpp
)

target_include_directories(onyx_render
    PUBLIC  ${PROJECT_SOURCE_DIR}/include
    PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(onyx_render
    PUBLIC  onyx_core
    PRIVATE onyx_utils
)
