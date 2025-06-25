cmake_minimum_required(VERSION 3.31)
project(poker)

find_package(tinyxml2 REQUIRED)

set(CMAKE_CXX_STANDARD 20)

add_executable(poker main.cpp
        Eval.cpp
        card.cpp
        card.h
        deck.h
        deck.cpp
        handrank.cpp
        handrank.h
        game.cpp
        game.h
        player.cpp
        player.h)

# Link SFML to target
target_link_libraries(poker PRIVATE sfml-graphics sfml-window sfml-system tinyxml2)