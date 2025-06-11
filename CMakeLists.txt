cmake_minimum_required(VERSION 3.24)

project(OpenGL_Tutorial)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(CMAKE_COLOR_DIAGNOSTICS ON)
# add_compile_options(-fdiagnostics-color=always -Wall -g)

file(GLOB_RECURSE SOURCES src/*.cpp src/*.c)

add_compile_definitions(SOURCE_DIR=\"${CMAKE_SOURCE_DIR}/src\")

add_compile_options(-Wall -g)
add_executable(OpenGL_Tutorial ${SOURCES})

target_include_directories(OpenGL_Tutorial PRIVATE
    ${CMAKE_SOURCE_DIR}/include
)

find_library(GLFW_LIB glfw3 HINTS ${CMAKE_SOURCE_DIR}/lib REQUIRED)

target_link_libraries(OpenGL_Tutorial PRIVATE ${GLFW_LIB})

if (APPLE)
    target_link_libraries(OpenGL_Tutorial PRIVATE
        "-framework Cocoa"
        "-framework IOKit"
        "-framework CoreFoundation"
        "-framework CoreVideo"
        "-framework OpenGL"
    )
endif()

set_target_properties(OpenGL_Tutorial PROPERTIES
    INSTALL_RPATH "@executable_path/lib"
)