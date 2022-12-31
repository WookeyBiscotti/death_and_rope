#include "ui_utf8.hpp"

void al::popBackUtf8(std::string& utf8) {
	if (utf8.empty())
		return;

	auto cp = utf8.data() + utf8.size();
	while (--cp >= utf8.data() && ((*cp & 0b10000000) && !(*cp & 0b01000000))) {}
	if (cp >= utf8.data())
		utf8.resize(cp - utf8.data());
}
