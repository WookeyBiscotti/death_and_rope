#pragma once

#include <functional>
#include <iostream>

#define IT_IS_TRUE(F)                                                                                    \
	do {                                                                                                 \
		if (!(F)) {                                                                                      \
			_al_test_context.out << "Expected true: " << #F << "; file: " << __FILE__ << ":" << __LINE__ << "\n"; \
			exit(1);                                                                                     \
		}                                                                                                \
	} while (false);

#define IT_IS_FALSE(F)                                                                                \
	if ((F)) {                                                                                        \
		_al_test_context.out << "Expected false: " << #F << "; file: " << __FILE__ << ":" << __LINE__ << "\n"; \
		exit(1);                                                                                      \
	}

#define IT_IS_EQ(F, S)                                                                                         \
	if ((F != S)) {                                                                                            \
		_al_test_context.out << "Expected eq, but: " << #F << " != " << #S << "; file: " << __FILE__ << ":" << __LINE__ \
		            << "\n";                                                                                   \
		exit(1);                                                                                               \
	}

#define AL_TEST_TRUE(F) IT_IS_TRUE((F))
#define AL_TEST_FALSE(F) IT_IS_FALSE((F))

namespace al {

class Context;

struct TestContext {
	std::ostream& out;
};

struct TestCaseArgs {
	void (*fn)(TestContext&);
	std::string_view msg;
	std::string_view fileName;
	int fileLine;

	bool operator==(const TestCaseArgs& o) const {
		return msg == o.msg && fileName == o.fileName && fileLine == o.fileLine;
	}
};

template<typename H>
H AbslHashValue(H h, const TestCaseArgs& m) {
	return H::combine(std::move(h), m.msg, m.fileName, m.fileLine);
}

int registerTestCase(const TestCaseArgs& arg);

void runTests();

void ALCH_IN_FRAME_Impl(std::function<void(Context&)> fn);

} // namespace al

template<class Fn>
void ALCH_IN_FRAME(Fn&& fn) {
	al::ALCH_IN_FRAME_Impl(fn);
}

#define ALCHEMY_MACRO_CAT_IMPL(STR1, STR2) STR1##STR2
#define ALCHEMY_MACRO_CAT(STR1, STR2)      ALCHEMY_MACRO_CAT_IMPL(STR1, STR2)
#define ALCH_TEST(MSG)                                                                                \
	inline static void ALCHEMY_MACRO_CAT(ALCHEMY_TEST_, __LINE__)(al::TestContext & _al_test_context);         \
	[[maybe_unused]] static int ALCHEMY_MACRO_CAT(ALCHEMY_TEST_UNUSED_VAR_, __LINE__) =               \
	    al::registerTestCase({&ALCHEMY_MACRO_CAT(ALCHEMY_TEST_, __LINE__), MSG, __FILE__, __LINE__}); \
	static void ALCHEMY_MACRO_CAT(ALCHEMY_TEST_, __LINE__)(al::TestContext & _al_test_context)
