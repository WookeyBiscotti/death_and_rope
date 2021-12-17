#include <engine/engine.hpp>

int main(int argc, const char** argv) {
	EngineConfig config;
	config.startScene = "dev_menu";

	Engine e;
	e.run(argv, argc, config);

	return 0;
}