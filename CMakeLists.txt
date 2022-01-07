cmake_minimum_required(VERSION 3.20)

project(SFML_Astro LANGUAGES CXX)

add_executable(astro Game.cpp main.cpp)
target_link_libraries(astro sfml-graphics sfml-window sfml-system)
