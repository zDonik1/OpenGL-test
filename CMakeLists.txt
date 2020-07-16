cmake_minimum_required(VERSION 3.5)

project(test LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(${PROJECT_NAME} main.cpp)

find_package(SFML 2.5 COMPONENTS system window REQUIRED)
#find_package(spdlog REQUIRED)

target_link_libraries(${PROJECT_NAME}
    GL
    GLEW
    SOIL
#    spdlog
    sfml-system
    sfml-window
)
