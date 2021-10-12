cmake_minimum_required(VERSION 3.0.0)
project(death_and_rope VERSION 0.1.0)

add_executable(death_and_rope_exe main.cpp world_cell.cpp)

add_subdirectory(3rdparty/SFML)

target_link_libraries(death_and_rope_exe PUBLIC sfml-graphics sfml-audio)