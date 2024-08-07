cmake_minimum_required(VERSION 3.28)
project(SwiftifySFML_v0_1)

set(CMAKE_CXX_STANDARD 17)

# Find the SFML package
find_package(SFML 2.5 COMPONENTS system window graphics REQUIRED)

add_executable(SwiftifySFML_v0_1 main.cpp
        Container.h
        Enumerations.h
        UIComponent.h
        Screen.h
        Application.h
        Container.cpp
        UIComponent.cpp
        Screen.cpp
        Application.cpp)

target_link_libraries(SwiftifySFML_v0_1 sfml-system sfml-window sfml-graphics)
