cmake_minimum_required(VERSION 3.17)
project(lab_1)

set(CMAKE_CXX_STANDARD 20)

include_directories(
        vendor/glad/include
)

add_executable(lab_1
        src/main.cpp
        src/graphics/glad.c
        src/util/types.h

        src/graphics/context.h
        src/graphics/context.cpp

        src/graphics/shader.h
        src/graphics/shader.cpp

        src/graphics/util/graphics_error.h
        src/object.cpp src/object.h
        src/normalized_object.cpp src/normalized_object.h src/camera.cpp src/camera.h src/line_strip.cpp src/line_strip.h)

set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)

add_subdirectory(vendor/glfw-3.3.2)
target_link_libraries(lab_1 glfw)

add_subdirectory(vendor/glm)
target_link_libraries(lab_1 glm)

find_package(OpenGL REQUIRED)
target_link_libraries(lab_1 OpenGL::GL)