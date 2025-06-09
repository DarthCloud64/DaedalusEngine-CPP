cmake_minimum_required(VERSION 3.16)
project(conspiracy)

set(CMAKE_CXX_STANDARD 23)
set(BGFX_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(BGFX_INSTALL OFF CACHE BOOL "" FORCE)

add_subdirectory(third_party/bgfx.cmake)
include_directories(third_party)

add_executable(conspiracy 
    src/main.cpp 
    src/ecs/ComponentStorage.cpp 
    src/utils/RenderUtils.cpp
    src/systems/CalculateTransformations.cpp
    src/systems/HandleInput.cpp
    src/systems/InitializeDependencies.cpp)
target_link_libraries(conspiracy bgfx bx bimg glfw)