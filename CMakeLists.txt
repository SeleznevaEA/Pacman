cmake_minimum_required(VERSION 3.28)
project(CMakeSFMLProject LANGUAGES CXX)
include(FetchContent)
set(BUILD_SHARED_LIBS OFF)
FetchContent_Declare(SFML GIT_REPOSITORY https://github.com/SFML/SFML.git GIT_TAG 3.0.2)
FetchContent_MakeAvailable(SFML)
add_executable(proga
        source/application.cpp
        source/application.h
        source/SelectState.h
        source/SelectState.cpp
        source/Resources.h
        source/GameState.h
        source/ExitState.h
        source/GameState.cpp
        source/ExitState.cpp
        source/main.cpp
        source/Menu.h
        source/Button.cpp
        source/Menu.cpp
        source/Commands.h
        source/Commands.cpp
        source/Maze.h
        source/Maze.cpp
        source/GameContext.h
        source/GameContext.cpp
        source/TextureCache.h
        source/TextureCache.cpp
        source/Animation.h
        source/Animation.cpp
        source/Builders.h
        source/Builders.cpp
)
set_target_properties(proga PROPERTIES CXX_STANDARD 17 CXX_STANDARD_REQUIRED ON CXX_EXTENSIONS ON)
target_link_libraries(proga PRIVATE SFML::System SFML::Window SFML::Graphics SFML::Audio)

add_custom_command(
        TARGET proga POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/assets
        $<TARGET_FILE_DIR:proga>/assets
)