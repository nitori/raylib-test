cmake_minimum_required(VERSION 4.0)
project(raylib_test C)

#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 ")

set(CMAKE_C_STANDARD 11)

set(RAYLIB_DIR "${CMAKE_SOURCE_DIR}/contrib/raylib-5.5_win64_mingw-w64")
set(FLECS_DIR "${CMAKE_SOURCE_DIR}/contrib/flecs")


set(SRC_FILES
        src/main.c
        "${FLECS_DIR}/flecs.c"
        src/components.c
)

if (CMAKE_BUILD_TYPE STREQUAL "Release")
    add_executable(raylib_test WIN32 ${SRC_FILES})
else ()
    add_executable(raylib_test ${SRC_FILES})
endif ()

# Headers
target_include_directories(raylib_test PRIVATE
        "${RAYLIB_DIR}/include"
        "${FLECS_DIR}"
        "src"
)

target_link_libraries(raylib_test PRIVATE
        "${RAYLIB_DIR}/lib/libraylibdll.a"
        ws2_32
        dbghelp
)


set_target_properties(raylib_test PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY_DEBUG "${CMAKE_BINARY_DIR}/build/Debug"
        RUNTIME_OUTPUT_DIRECTORY_RELEASE "${CMAKE_BINARY_DIR}/build/Release"
)

if (WIN32)
    set(_DLLS
            "${RAYLIB_DIR}/lib/raylib.dll"
    )

    foreach (dll IN LISTS _DLLS)
        add_custom_command(TARGET raylib_test POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E copy_if_different
                "${dll}" $<TARGET_FILE_DIR:raylib_test>)
    endforeach ()
endif ()

