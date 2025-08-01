cmake_minimum_required(VERSION 3.16)
project(Editor)

find_package(OpenGL REQUIRED)

add_subdirectory(vendor/glfw EXCLUDE_FROM_ALL)

set(GLAD_SOURCES 
    vendor/glad/src/glad.c
)
set(GLAD_HEADERS 
    vendor/glad/include/glad/glad.h
    vendor/glad/include/KHR/khrplatform.h
)


file(GLOB_RECURSE EDITOR_SOURCES 
    "src/*.cpp"
    "src/*.c"
)

file(GLOB_RECURSE EDITOR_HEADERS 
    "src/*.h"
    "src/*.hpp"
)

add_executable(Editor 
    ${EDITOR_SOURCES} 
    ${EDITOR_HEADERS}
    ${GLAD_SOURCES}
    ${GLAD_HEADERS}
 "src/Editor.h")

include_directories(src)

target_include_directories(Editor PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/src
    ${CMAKE_CURRENT_SOURCE_DIR}/vendor
    ${CMAKE_CURRENT_SOURCE_DIR}/vendor/glad/include
    ${CMAKE_SOURCE_DIR}/Core/src
    ${CMAKE_SOURCE_DIR}/Core/vendor
)

target_link_libraries(Editor PRIVATE
    Core
    OpenGL::GL
    glfw
)

if(WIN32)
    target_link_libraries(Editor PRIVATE
        user32
        gdi32
        shell32
    )
elseif(UNIX AND NOT APPLE)
    target_link_libraries(Editor PRIVATE
        dl
        pthread
    )
elseif(APPLE)
    find_library(COCOA_LIBRARY Cocoa)
    find_library(IOKIT_LIBRARY IOKit)
    find_library(COREVIDEO_LIBRARY CoreVideo)
    target_link_libraries(Editor PRIVATE
        ${COCOA_LIBRARY}
        ${IOKIT_LIBRARY}
        ${COREVIDEO_LIBRARY}
    )
endif()

# find_package(imgui REQUIRED)
# target_link_libraries(Editor PRIVATE imgui::imgui)

set_target_properties(Editor PROPERTIES
    VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}"
)

set_target_properties(Editor PROPERTIES FOLDER "Editor")

target_compile_definitions(Editor PRIVATE
    GLFW_INCLUDE_NONE
)

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    target_compile_definitions(Editor PRIVATE DEBUG)
else()
    target_compile_definitions(Editor PRIVATE RELEASE)
endif()