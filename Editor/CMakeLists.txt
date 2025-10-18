project(Editor)

add_executable(Editor)

file(GLOB_RECURSE EDITOR_SOURCES "src/*.cpp")
file(GLOB_RECURSE EDITOR_HEADERS "src/*.h")

target_sources(Editor PRIVATE
    ${EDITOR_SOURCES}
    ${EDITOR_HEADERS}
)

target_include_directories(Editor PRIVATE
    src
    ../Core/src
    ../Core/vendor/glm
    ../Core/vendor/glfw/include
    ../Core/vendor/glad/include
    ../Core/vendor/imgui
)

# Link libraries
target_link_libraries(Editor PRIVATE Core)

if(WIN32)
    target_link_libraries(Editor PRIVATE opengl32)
endif()

# Configuration-specific settings
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    target_compile_definitions(Editor PRIVATE DEBUG)
elseif(CMAKE_BUILD_TYPE STREQUAL "Release")
    target_compile_definitions(Editor PRIVATE RELEASE)
elseif(CMAKE_BUILD_TYPE STREQUAL "Dist")
    target_compile_definitions(Editor PRIVATE DIST)
endif()

source_group("Headers" FILES ${EDITOR_HEADERS})
source_group("Sources" FILES ${EDITOR_SOURCES})

add_dependencies(Editor Core)