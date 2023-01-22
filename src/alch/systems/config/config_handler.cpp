#include "config_handler.hpp"

#include "config.hpp"

using namespace al;

ConfigHandler::~ConfigHandler() {
	_system.unsubscribe(*this);
}

void ConfigHandler::subscribe(const String& event, std::function<void(const String&)> fn) {
	_system.subscribe(*this, event, std::move(fn));
}

void ConfigHandler::unsubscribe(const String& event) {
	_system.unsubscribe(*this, event);
}

void ConfigHandler::unsubscribe() {
	_system.unsubscribe(*this);
}
