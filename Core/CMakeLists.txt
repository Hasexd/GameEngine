project(Core)

add_library(Core STATIC)

file(GLOB_RECURSE CORE_SOURCES "src/*.cpp")
file(GLOB_RECURSE CORE_HEADERS "src/*.h")
file(GLOB_RECURSE GLM_HEADERS "vendor/glm/*/*.hpp" "vendor/glm/*/*.h" "vendor/glm/*/*.inl")
file(GLOB GLAD_SOURCES "vendor/glad/src/*.c")
file(GLOB GLFW_SOURCES "vendor/glfw/src/*.c")
file(GLOB_RECURSE IMGUI_SOURCES "vendor/imgui/*.cpp")
file(GLOB_RECURSE IMGUI_HEADERS "vendor/imgui/*.h")
file(GLOB_RECURSE OBJ_LOADER_HEADERS "vendor/obj-loader/Source/*.h")
file(GLOB_RECURSE SHADERS "shaders/*.glsl")

target_sources(Core PRIVATE
    ${CORE_SOURCES}
    ${CORE_HEADERS}
    ${GLM_HEADERS}
    ${GLAD_SOURCES}
    ${GLFW_SOURCES}
    ${IMGUI_SOURCES}
    ${IMGUI_HEADERS}
    ${OBJ_LOADER_HEADERS}
    ${SHADERS}
)

target_include_directories(Core PUBLIC
    src
    vendor/glm
    vendor/glad/include
    vendor/glfw/include
    vendor/imgui
    vendor/obj-loader/Source
)

if(WIN32)
    target_compile_definitions(Core PRIVATE
        _CRT_SECURE_NO_WARNINGS
        _GLFW_WIN32
        IMGUI_IMPL_OPENGL_LOADER_GLAD
    )
    target_link_libraries(Core PUBLIC opengl32)
elseif(UNIX AND NOT APPLE)
    target_compile_definitions(Core PRIVATE
        _CRT_SECURE_NO_WARNINGS
        _GLFW_X11
        IMGUI_IMPL_OPENGL_LOADER_GLAD
    )
    target_link_libraries(Core PUBLIC
        GL
        X11
        pthread
        Xrandr
        Xi
        dl
    )
endif()

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    target_compile_definitions(Core PRIVATE DEBUG)
elseif(CMAKE_BUILD_TYPE STREQUAL "Release")
    target_compile_definitions(Core PRIVATE RELEASE)
elseif(CMAKE_BUILD_TYPE STREQUAL "Dist")
    target_compile_definitions(Core PRIVATE DIST)
endif()

source_group("Headers" FILES ${CORE_HEADERS})
source_group("Sources" FILES ${CORE_SOURCES})
source_group("Shaders" FILES ${SHADERS})

add_custom_command(TARGET Core POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E make_directory $<TARGET_FILE_DIR:Core>/shaders
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_CURRENT_SOURCE_DIR}/shaders
        $<TARGET_FILE_DIR:Core>/shaders
    COMMENT "Copying shaders to output directory"
)