#pragma once

#include <iostream>

#define IT_IS_TRUE(F)                                                                                \
	if (!(F)) {                                                                                      \
		context.out << "Expected true: " << #F << "; file: " << __FILE__ << ":" << __LINE__ << "\n"; \
		exit(1);                                                                                     \
	}

#define IT_IS_FALSE(F)                                                                                \
	if ((F)) {                                                                                        \
		context.out << "Expected false: " << #F << "; file: " << __FILE__ << ":" << __LINE__ << "\n"; \
		exit(1);                                                                                      \
	}

#define IT_IS_EQ(F, S)                                                                                         \
	if ((F != S)) {                                                                                            \
		context.out << "Expected eq, but: " << #F << " != " << #S << "; file: " << __FILE__ << ":" << __LINE__ \
		            << "\n";                                                                                   \
		exit(1);                                                                                               \
	}

namespace al {

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

template<class Fn>
void ALCH_IN_FRAME(Fn&& fn) {
	
	fn();
}

} // namespace al

#define ALCHEMY_MACRO_CAT_IMPL(STR1, STR2) STR1##STR2
#define ALCHEMY_MACRO_CAT(STR1, STR2)      ALCHEMY_MACRO_CAT_IMPL(STR1, STR2)
#define ALCH_TEST(MSG)                                                                                \
	inline static void ALCHEMY_MACRO_CAT(ALCHEMY_TEST_, __LINE__)(al::TestContext & context);         \
	[[maybe_unused]] static int ALCHEMY_MACRO_CAT(ALCHEMY_TEST_UNUSED_VAR_, __LINE__) =               \
	    al::registerTestCase({&ALCHEMY_MACRO_CAT(ALCHEMY_TEST_, __LINE__), MSG, __FILE__, __LINE__}); \
	static void ALCHEMY_MACRO_CAT(ALCHEMY_TEST_, __LINE__)(al::TestContext & context)
