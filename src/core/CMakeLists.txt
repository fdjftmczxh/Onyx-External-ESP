add_library(onyx_core STATIC
    esp_manager.cpp
)

target_include_directories(onyx_core
    PUBLIC  ${PROJECT_SOURCE_DIR}/include
    PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(onyx_core
    PUBLIC  onyx_utils
)
