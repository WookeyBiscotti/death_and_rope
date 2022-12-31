#pragma once

#include <string>

namespace al {

constexpr auto UTF8_SPECIAL_SYMBOL1_FIRST = 0;
constexpr auto UTF8_SPECIAL_SYMBOL1_LAST = 31;

constexpr auto UTF8_SPECIAL_SYMBOL2_FIRST = 128;
constexpr auto UTF8_SPECIAL_SYMBOL2_LAST = 159;

constexpr auto UTF8_SPECIAL_SYMBOL_TAB = 9;
constexpr auto UTF8_SPECIAL_SYMBOL_SPACE = 32;
constexpr auto UTF8_SPECIAL_SYMBOL_DEL = 127;
constexpr auto UTF8_SPECIAL_SYMBOL_BACKSPACE = 8;

void popBackUtf8(std::string& utf8);

} // namespace al
