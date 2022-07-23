#include "localization.hpp"

Localization::Localization(Context& context, Path localoiationFile): _context(context) {
	setAsGlobal();
}

void Localization::language(Language lang) {
	if (lang != _currentLanguage) {
		for (auto [_, v] : _updatedState) {
			*v = false;
		}
	}
	_currentLanguage = lang;
}

void Localization::setAsGlobal() {
	_global = this;
}

Localization* Localization::global() {
	return _global;
}

std::string_view Localization::translate(std::string_view msg) {
	if (auto found = _langs.find(msg); found != _langs.end()) {
		return found->second[_currentLanguage];
	} else {
		return msg;
	}
}
