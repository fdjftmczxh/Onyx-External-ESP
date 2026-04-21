add_executable(${PROJECT_NAME}
    main.cpp
)

target_link_libraries(${PROJECT_NAME}
    PRIVATE
        onyx_render
        onyx_core
        AImGui
        KittyMemoryEx
        android
        log
        GLESv3
        EGL
)

target_compile_features(${PROJECT_NAME} PUBLIC cxx_std_20)
