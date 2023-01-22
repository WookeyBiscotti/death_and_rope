cmake_minimum_required(VERSION 3.16.3)

project(
  AlchemyEngine
  VERSION 0.1.0
  LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)

add_library(AlchemyEngineLib STATIC)

set(SDL_STATIC TRUE)
set(SKIP_PERFORMANCE_COMPARISON TRUE)
set(ABSL_ENABLE_INSTALL TRUE)

set(CMAKE_CXX_FLAGS -stdlib=libc++)

add_subdirectory(3rdparty/SFML)
add_subdirectory(3rdparty/json)
add_subdirectory(3rdparty/cereal)
add_subdirectory(3rdparty/spdlog)
add_subdirectory(3rdparty/Catch2)
add_subdirectory(3rdparty/AABBTree)
add_subdirectory(3rdparty/box2d)
add_subdirectory(3rdparty/ChaiScript)
# add_subdirectory(3rdparty/SDL)
add_subdirectory(3rdparty/sol2)
add_subdirectory(3rdparty/abseil-cpp)
add_subdirectory(3rdparty/cpp-terminal)

target_compile_definitions(AlchemyEngineLib PUBLIC B2_USER_SETTINGS)

file(
  GLOB_RECURSE MAIN_SOURCES
  RELATIVE "${CMAKE_CURRENT_LIST_DIR}"
  CONFIGURE_DEPENDS "${CMAKE_CURRENT_LIST_DIR}/src/*.[ch]pp")

file(
  GLOB_RECURSE TESTS_SOURCES
  RELATIVE "${CMAKE_CURRENT_LIST_DIR}"
  CONFIGURE_DEPENDS "${CMAKE_CURRENT_LIST_DIR}/tests/*.cpp")

set(DEPS_SOURCES
    3rdparty/imgui/imgui_tables.cpp 3rdparty/imgui/imgui_widgets.cpp
    3rdparty/imgui/imgui_draw.cpp 3rdparty/imgui/imgui.cpp
    3rdparty/imgui-sfml/imgui-SFML.cpp 3rdparty/imgui/misc/cpp/imgui_stdlib.cpp)

#TODO: make build commands for luaJit: make CFLAGS="-fPIC" make install PREFIX=/home/alex/code/game/3rdparty/death_and_rope/3rdparty/LuaJIT/LuaJIT

set(INCLUDES_DEPS_DIRS
    src
    3rdparty/imgui
    3rdparty/imgui-sfml
    3rdparty/imgui-filebrowser
    3rdparty/imgui/misc/cpp
    3rdparty/box2d/include
    3rdparty/lua
    3rdparty/bgfx/include
    3rdparty/bx/include
    3rdparty/ChaiScript/include
    3rdparty/LuaJIT/LuaJIT/include
    3rdparty/LuaJIT/LuaJIT/include/luajit-2.1
    )

set(DEPS_LIBS
    sfml-graphics
    sfml-audio
    nlohmann_json
    cereal::cereal
    spdlog::spdlog
    aabb_tree
    box2d
    # bgfxRelease
    absl::base
    absl::flat_hash_set
    absl::flat_hash_map
    absl::btree
    absl::inlined_vector
    # SDL2::SDL2-static
    cpp-terminal::cpp-terminal
    sol2::sol2
    libluajit-5.1.a
    ${CMAKE_DL_LIBS})

set(ALCH_DEPS_LIBS_PATH 3rdparty/LuaJIT/LuaJIT/lib)

if(APPLE)
  list(APPEND DEPS_LIBS "-framework OpenGL" "-framework GLUT")
elseif(UNIX)
  list(APPEND DEPS_LIBS GL GLU)
  # list(APPEND ALCH_DEPS_LIBS_PATH
  #      ${CMAKE_CURRENT_LIST_DIR}/3rdparty/bgfx/.build/linux64_gcc/bin)
endif()

target_link_libraries(AlchemyEngineLib PUBLIC ${DEPS_LIBS})

target_sources(AlchemyEngineLib PUBLIC ${MAIN_SOURCES} ${DEPS_SOURCES})
target_include_directories(AlchemyEngineLib PUBLIC ${INCLUDES_DEPS_DIRS})
target_include_directories(AlchemyEngineLib
                           PUBLIC ${CMAKE_CURRENT_LIST_DIR}/src)
target_link_directories(AlchemyEngineLib PUBLIC ${ALCH_DEPS_LIBS_PATH})

add_library(AlchemyEngineLib::AlchemyEngineLib ALIAS AlchemyEngineLib)

# add_executable(death_and_rope_exe game/main.cpp)
# target_link_libraries(death_and_rope_exe PUBLIC death_and_rope_lib)

# add_executable(death_and_rope_tests ${TESTS_SOURCES})
# target_link_libraries(death_and_rope_tests PUBLIC death_and_rope_lib
# Catch2::Catch2WithMain)

set(PROD_BUILD
    OFF
    CACHE BOOL "make prod build without logging/debug/imgui for performance")

# if(${PROD_BUILD}) target_compile_definitions(death_and_rope_exe PUBLIC
# PROD_BUILD) endif()

file(
  COPY ${CMAKE_CURRENT_SOURCE_DIR}/src/alch/systems/physics/box2d_settings/b2_user_settings.h
  DESTINATION ${CMAKE_CURRENT_SOURCE_DIR}/3rdparty/box2d/include/box2d)

add_executable(AlchemyEngineLibBuiltinTests "${CMAKE_CURRENT_LIST_DIR}/tests/builtin_tests.cpp")
target_link_libraries(AlchemyEngineLibBuiltinTests PUBLIC AlchemyEngineLib)


add_executable(AlchemyEngineLibUiExperiments "${CMAKE_CURRENT_LIST_DIR}/tests/ui_experiments/main.cpp")
target_link_libraries(AlchemyEngineLibUiExperiments PUBLIC AlchemyEngineLib)



