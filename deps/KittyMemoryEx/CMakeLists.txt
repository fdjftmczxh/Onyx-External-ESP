# deps/KittyMemoryEx/CMakeLists.txt
add_library(KittyMemoryEx STATIC
    KittyAsm.cpp
    KittyIOFile.cpp
    KittyMemOp.cpp
    KittyMemoryEx.cpp
    KittyMemoryMgr.cpp
    KittyPerfEvent.cpp
    KittyPtrValidator.cpp
    KittyScanner.cpp
    KittyTrace.cpp
    KittyUtils.cpp
    MemoryBackup.cpp
    MemoryPatch.cpp
)

target_include_directories(KittyMemoryEx
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}
)

# KittyMemoryEx might need keystone for KittyAsm
set(KEYSTONE_LIB ${CMAKE_CURRENT_SOURCE_DIR}/deps/Keystone/libs-android/${ANDROID_ABI}/libkeystone.a)
target_link_libraries(KittyMemoryEx PRIVATE ${KEYSTONE_LIB})
