#pragma once

#include "alch/common/archive.hpp"
#include "alch/common/hash.hpp"
#include "alch/common/types.hpp"
#include "alch/engine/system.hpp"
//
#include <array>
#include <deque>
#include <memory>
#include <string_view>
#include <unordered_map>

namespace al {

class Context;

class Localization: public System {
  public:
	enum Language { EN = 0, RU, LANGS_COUNT };

	explicit Localization(Context& context, Path localisationFile);

	// translate to current language
	std::string_view translate(const char* msg);

	Language language() const { return _currentLanguage; };

	void language(Language lang);

	void setAsGlobal();

	static Localization* global();

  private:
	inline static Localization* _global = nullptr;

	Context& _context;

	Language _currentLanguage = EN;

	std::array<std::vector<char>, LANGS_COUNT> _textBlob;
	std::unordered_map<std::string_view, std::array<std::string_view, LANGS_COUNT>> _langs;
	std::unordered_map<const char*, std::string_view> _translate;
};

}

#define ALTR(LANG, MSG) (Localization::LANG, Localization::global()->translate(MSG))
#define cALTR(LANG, MSG) (Localization::LANG, Localization::global()->translate(MSG).data())
