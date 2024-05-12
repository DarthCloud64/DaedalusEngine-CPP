cmake_minimum_required(VERSION 3.22.1)
project(DaedalusEngine)

set(CMAKE_CXX_STANDARD 17)

add_executable(DaedalusEngine main.cpp
        Application/Application.cpp
        Application/Application.h
        Window/WindowManager.cpp
        Window/WindowManager.h
        Input/InputManager.cpp
        Input/InputManager.h
        Input/Input.h
        Audio/AudioManager.cpp
        Audio/AudioManager.h
        Game/Container.h
        Game/Component.h
        Game/MusicComponent.h
        Game/AudioComponent.h
        Graphics/RenderingManager.cpp
        Graphics/RenderingManager.h
        Resource/ResourceLoader.cpp
        Resource/ResourceLoader.h
        Application/ApplicationState.h
        Utilities/Utilities.cpp
        Utilities/Utilities.h
        Graphics/Vertex.h)

target_link_libraries(DaedalusEngine assimp-vc143-mtd glfw3)