cmake_minimum_required(VERSION 3.16)
project(Core)

add_subdirectory(vendor/glm EXCLUDE_FROM_ALL)

include_directories(src)

file(GLOB_RECURSE CORE_SOURCES 
    "src/*.cpp"
)

file(GLOB_RECURSE CORE_HEADERS 
    "src/*.h"
)

file(GLOB_RECURSE VENDOR_SOURCES 
    "vendor/*.cpp"
    "vendor/*.c"
)

file(GLOB_RECURSE VENDOR_HEADERS 
    "vendor/*.h"
    "vendor/*.hpp"
)

add_library(Core STATIC 
    ${CORE_SOURCES} 
    ${CORE_HEADERS}
    ${VENDOR_SOURCES}
    ${VENDOR_HEADERS}
)

target_include_directories(Core PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/src
    ${CMAKE_CURRENT_SOURCE_DIR}/vendor
)

target_link_libraries(Core PUBLIC
    glm::glm
)


if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    target_compile_definitions(Core PUBLIC DEBUG)
else()
    target_compile_definitions(Core PUBLIC RELEASE)
endif()

set_target_properties(Core PROPERTIES FOLDER "Core")