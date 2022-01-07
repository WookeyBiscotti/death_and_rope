cmake_minimum_required(VERSION 3.18.0)

project(death_and_rope VERSION 0.1.0)

set(CMAKE_CXX_STANDARD 20)

add_executable(death_and_rope_exp main.cpp world_cell.cpp)

add_subdirectory(3rdparty/SFML)
add_subdirectory(3rdparty/json)
add_subdirectory(3rdparty/cereal)
add_subdirectory(3rdparty/spdlog)
add_subdirectory(3rdparty/Catch2)
add_subdirectory(3rdparty/AABBTree)
add_subdirectory(3rdparty/box2d)
add_subdirectory(3rdparty/sol2)

add_compile_definitions(B2_USER_SETTINGS)

target_link_libraries(death_and_rope_exp PUBLIC sfml-graphics sfml-audio)

set(MAIN_SOURCES
    src/rope.cpp
    src/engine/engine.cpp
    src/world.cpp
    src/scenes/main_menu.cpp
    src/scenes/dev_menu.cpp
    src/scenes/world_editor.cpp
    src/systems/render/render.cpp
    src/systems/render/sprite_component.cpp
    src/systems/render/drawable.cpp
    src/systems/render/camera.cpp
    src/systems/logging/logger.cpp
    src/systems/assets/asset_cache.cpp
    src/systems/assets/texture.cpp
    src/systems/window/window.cpp
    src/systems/imgui/imgui_system.cpp
    src/systems/broker/receiver.cpp
    src/systems/broker/sender.cpp
    src/systems/debug/debug_system.cpp
    src/systems/config/config.cpp
    src/systems/transform/transform.cpp
    src/systems/group/group.cpp
    src/systems/physics/physics.cpp
    src/systems/physics/body.cpp
    src/systems/physics/collider.cpp
    src/systems/scenes/scene_system.cpp
    src/systems/names/name.cpp
    src/systems/scripts/scripts.cpp
    )

set(TESTS_SOURCES src/tests/engine/entity_tests.cpp
                  src/tests/engine/broker_tests.cpp)

set(DEPS_SOURCES
    3rdparty/imgui/imgui_tables.cpp 3rdparty/imgui/imgui_widgets.cpp
    3rdparty/imgui/imgui_draw.cpp 3rdparty/imgui/imgui.cpp
    3rdparty/imgui-sfml/imgui-SFML.cpp 3rdparty/imgui/misc/cpp/imgui_stdlib.cpp)

set(INCLUDES_DEPS_DIRS
    src 3rdparty/imgui 3rdparty/imgui-sfml 3rdparty/imgui-filebrowser
    3rdparty/imgui/misc/cpp 3rdparty/box2d/include 3rdparty/lua)

set(RAW_LIBS_PATH 3rdparty/lua)

set(DEPS_LIBS
    GL
    GLU
    sfml-graphics
    sfml-audio
    nlohmann_json
    cereal::cereal
    spdlog::spdlog
    sol2::sol2
    aabb_tree
    box2d 
    lua
    ${CMAKE_DL_LIBS}
    )

add_executable(death_and_rope_exe src/game/main.cpp ${MAIN_SOURCES}
    ${DEPS_SOURCES})
target_link_directories(death_and_rope_exe PUBLIC ${RAW_LIBS_PATH})
target_include_directories(death_and_rope_exe PUBLIC ${INCLUDES_DEPS_DIRS})
target_link_libraries(death_and_rope_exe PUBLIC ${DEPS_LIBS})


add_executable(death_and_rope_tests ${TESTS_SOURCES} ${MAIN_SOURCES}
                                    ${DEPS_SOURCES})
target_include_directories(death_and_rope_tests PUBLIC ${INCLUDES_DEPS_DIRS})
target_link_libraries(death_and_rope_tests PUBLIC Catch2::Catch2WithMain
                                                  ${DEPS_LIBS})

set(PROD_BUILD
    OFF
    CACHE BOOL "make prod build without logging/debug/imgui for performance")

if(${PROD_BUILD})
  target_compile_definitions(death_and_rope_exe PUBLIC PROD_BUILD)
endif()

file(
  COPY ${CMAKE_CURRENT_SOURCE_DIR}/src/systems/physics/box2d_settings/b2_user_settings.h
  DESTINATION ${CMAKE_CURRENT_SOURCE_DIR}/3rdparty/box2d/include/box2d)
