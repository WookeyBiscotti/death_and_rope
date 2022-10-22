#include "localization.hpp"

#include "alch/common/json.hpp"
#include "alch/engine/context.hpp"
#include "alch/systems/filesystem/filesystem.hpp"

using namespace al;

Localization::Language strToLang(const std::string& l) {
	std::pair<const std::string, Localization::Language> am[] = {
	    {"RU", Localization::Language::RU}, {"EN", Localization::Language::EN}};

	static_assert(sizeof(am) / sizeof(decltype(am[0])) == Localization::Language::LANGS_COUNT, "Wrong languages count");

	static const std::unordered_map<std::string, Localization::Language> m(&am[0], &am[0] + Localization::LANGS_COUNT);

	if (auto found = m.find(l); found != m.end()) {
		return found->second;
	} else {
		return Localization::LANGS_COUNT;
	}
}

Localization::Localization(Context& context, Path localisationFile): System(context) {
	setAsGlobal();

	auto str = context.systemRef<FileSystem>().load(localisationFile);

	auto js = Json::parse(str, nullptr, false);

	std::array<size_t, Localization::LANGS_COUNT> blobSizes{};

	if (auto usedJs = js.find("used"); usedJs != js.end()) {
		for (auto p : *usedJs) {
			if (auto originJs = p.find("origin");
			    originJs != p.end() && strToLang(*originJs) != Localization::LANGS_COUNT) {
				if (auto textJs = p.find("text"); textJs != p.end()) {
					for (auto [lstr, text] : textJs->items()) {
						if (auto lang = strToLang(lstr); lang != Localization::LANGS_COUNT) {
							blobSizes[lang] += text.get<std::string>().size() + 1;
						}
					}
				}
			}
		}

		for (size_t l = 0; l != Localization::LANGS_COUNT; ++l) {
			_textBlob[l].reserve(blobSizes[l]);
		}

		for (auto p : *usedJs) {
			if (auto originJs = p.find("origin");
			    originJs != p.end() && strToLang(*originJs) != Localization::LANGS_COUNT) {
				if (auto textJs = p.find("text"); textJs != p.end()) {
					std::array<std::string_view, LANGS_COUNT> ps{};
					for (auto [lstr, text] : textJs->items()) {
						if (auto lang = strToLang(lstr); lang != Localization::LANGS_COUNT) {
							const auto str = text.get<std::string_view>();
							_textBlob[lang].insert(_textBlob[lang].end(), str.begin(), str.end());
							ps[lang] = std::string_view(&*(_textBlob[lang].end() - str.size()), str.size());
						}
					}
					_langs[ps[strToLang(*originJs)]] = ps;
				}
			}
		}
	}
}

void Localization::language(Language lang) {
	if (lang != _currentLanguage) {
		_translate.clear();
	}

	_currentLanguage = lang;
}

void Localization::setAsGlobal() {
	_global = this;
}

Localization* Localization::global() {
	return _global;
}

std::string_view Localization::translate(const char* msg) {
	if (auto found = _translate.find(msg); found != _translate.end()) {
		return found->second;
	} else {
		if (auto found = _langs.find(msg); found != _langs.end()) {
			auto str = found->second[_currentLanguage];
			_translate[msg] = str;
			return str;
		} else {
			return msg;
		}
	}
}
