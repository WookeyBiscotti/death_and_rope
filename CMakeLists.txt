cmake_minimum_required(VERSION 3.0.0)

project(death_and_rope VERSION 0.1.0)

set(CMAKE_CXX_STANDARD 20)

add_executable(death_and_rope_exp main.cpp world_cell.cpp)

add_subdirectory(3rdparty/SFML)
add_subdirectory(3rdparty/json)
add_subdirectory(3rdparty/cereal)

target_link_libraries(death_and_rope_exp PUBLIC sfml-graphics sfml-audio)

set(MAIN_SOURCES
    src/main.cpp
    src/texture.cpp
    src/rope.cpp
    src/sprite.cpp
    src/asset_cache.cpp
    src/engine.cpp
    src/scenes/main_menu.cpp
    src/scenes/dev_menu.cpp
    src/scenes/sprite_editor.cpp
)

set(DEPS_SOURCES
    3rdparty/imgui/imgui_tables.cpp
    3rdparty/imgui/imgui_widgets.cpp
    3rdparty/imgui/imgui_draw.cpp
    3rdparty/imgui/imgui.cpp
    
    3rdparty/imgui-sfml/imgui-SFML.cpp
)

add_executable(death_and_rope_exe ${MAIN_SOURCES} ${DEPS_SOURCES})

target_include_directories(death_and_rope_exe PUBLIC src 3rdparty/imgui 3rdparty/imgui-sfml  3rdparty/imgui-filebrowser)

target_link_libraries(death_and_rope_exe PUBLIC GL GLU sfml-graphics sfml-audio nlohmann_json cereal::cereal)

file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/assets/ DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/assets/)