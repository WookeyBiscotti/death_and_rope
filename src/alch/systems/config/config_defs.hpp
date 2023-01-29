#pragma once

#include "alch/common/containers/string.hpp"
#include "alch/systems/config/config_value.hpp"

namespace al::config {

template<class T, const char C[]>
static const T defaultValue;

#define DEFAULT(NAME, TYPE, VALUE) \
	template<>                     \
	static const ::al::ConfigValue defaultValue<TYPE, NAME> = VALUE;

constexpr char APPLICATION_NAME[] = "application.name";
DEFAULT(APPLICATION_NAME, String, "Alchemy Engine Application")

constexpr char CONFIG_PATH[] = "cofig.path";
DEFAULT(CONFIG_PATH, String, "")

constexpr char RESOURCES_PATH[] = "resources.path";
DEFAULT(RESOURCES_PATH, String, "")

constexpr char USER_DATA_PATH[] = "user.data.path";
DEFAULT(USER_DATA_PATH, String, "")

constexpr char WINDOW_SIZE[] = "window.size";
DEFAULT(WINDOW_SIZE, Vector2f, Vector2f(800.0f, 600.0f));

constexpr char WINDOW_POSITION_X[] = "window.position.x";
constexpr char WINDOW_POSITION_Y[] = "window.position.y";
constexpr char WINDOW_POSITION[] = "window.position";

constexpr char WINDOW_FULLSCREEN[] = "window.fullscreen";
DEFAULT(WINDOW_FULLSCREEN, bool, true)
constexpr char WINDOW_BORDERLESS[] = "window.borderless";
constexpr char WINDOW_VERTICAL_SYNC[] = "window.vertical_sync";

constexpr char ENABLE_DEFAULT_SCENES[] = "enable_default_scenes";
DEFAULT(ENABLE_DEFAULT_SCENES, bool, false)

#undef DEFAULT
} // namespace al::config
