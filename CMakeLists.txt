cmake_minimum_required(VERSION 3.0.0)

project(death_and_rope VERSION 0.1.0)

set(CMAKE_CXX_STANDARD 20)

add_executable(death_and_rope_exp main.cpp world_cell.cpp)

add_subdirectory(3rdparty/SFML)
add_subdirectory(3rdparty/json)
add_subdirectory(3rdparty/cereal)
add_subdirectory(3rdparty/spdlog)
# add_subdirectory(3rdparty/ctti)

target_link_libraries(death_and_rope_exp PUBLIC sfml-graphics sfml-audio)

set(MAIN_SOURCES
    src/main.cpp
    src/rope.cpp
    src/engine.cpp
    src/world.cpp
    src/scenes/main_menu.cpp
    src/scenes/dev_menu.cpp
    src/scenes/sprite_editor.cpp
    src/scenes/sprites_view.cpp
    src/scenes/world_editor.cpp
    
    src/systems/render/render.cpp
    src/systems/render/sprite_component.cpp
    
    src/systems/logging/logger.cpp
    
    src/systems/assets/asset_cache.cpp
    src/systems/assets/texture.cpp
    src/systems/assets/sprite.cpp
    
    src/systems/window/window.cpp
    
    src/systems/imgui/imgui_system.cpp

    src/systems/broker/receiver.cpp
    src/systems/broker/sender.cpp

    src/systems/debug/debug_system.cpp
 
    src/systems/config/config.cpp
)

set(DEPS_SOURCES
    3rdparty/imgui/imgui_tables.cpp
    3rdparty/imgui/imgui_widgets.cpp
    3rdparty/imgui/imgui_draw.cpp
    3rdparty/imgui/imgui.cpp
    
    3rdparty/imgui-sfml/imgui-SFML.cpp
    3rdparty/imgui/misc/cpp/imgui_stdlib.cpp
)

add_executable(death_and_rope_exe ${MAIN_SOURCES} ${DEPS_SOURCES})

target_include_directories(death_and_rope_exe PUBLIC src 3rdparty/imgui 3rdparty/imgui-sfml  3rdparty/imgui-filebrowser 3rdparty/imgui/misc/cpp)

target_link_libraries(death_and_rope_exe PUBLIC GL GLU sfml-graphics sfml-audio nlohmann_json cereal::cereal spdlog::spdlog)

file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/assets/ DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/assets/)

set(PROD_BUILD OFF CACHE BOOL "make prod build withput loging for perfotmance")

if(${PROD_BUILD})
    target_compile_definitions(death_and_rope_exe PUBLIC PROD_BUILD)
endif()