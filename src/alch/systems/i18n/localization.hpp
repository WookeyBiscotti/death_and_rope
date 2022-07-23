#pragma once

#include "alch/common/archive.hpp"
#include "alch/common/types.hpp"
#include "alch/engine/system.hpp"
//
#include <array>
#include <deque>
#include <memory>
#include <string_view>
#include <unordered_map>

class Context;

class Localization: public System {
  public:
	enum Language { EN = 0, RU, LANGS_COUNT };

	explicit Localization(Context& context, Path localoiationFile);

	// std::string_view translate(std::string_view msg, Language lang);

	// translate to current language
	std::string_view translate(std::string_view msg);

	Language language() const { return _currentLanguage; };

	void language(Language lang);

	void setAsGlobal();

	static Localization* global();

	// template<const char*, size_t>
	// std::string_view _translateWithStaticState(std::string_view msg) {
	// 	static bool updated = false;
	// 	static std::string_view str;
	// 	if (!updated) {
	// 		_updatedState[msg] = &updated;
	// 		updated = true;
	// 		if (auto found = _langs.find(msg); found != _langs.end()) {
	// 			str = found->second[_currentLanguage];
	// 		} else {
	// 			str = msg;
	// 		}
	// 	}

	// 	return str;
	// }

  private:
	inline static Localization* _global = nullptr;

	Context& _context;

	Language _currentLanguage = EN;
	std::unordered_map<std::string_view /*text in code*/, std::array<std::string_view, LANGS_COUNT>> _langs;
	std::unordered_map<std::string_view /*text in code*/, bool*> _updatedState;
};

// #define ALTR(MSG, LANG) Localization::global()->_translateWithStaticState<__FILE__, __LINE__>(MSG)
#define ALTR(MSG, LANG) (Localization::LANG ,Localization::global()->translate(MSG))
