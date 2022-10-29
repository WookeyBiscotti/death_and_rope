#pragma once

#include <iostream>

#define INLINE_TESTER(MODULE_NAME, TEST_NAME)                                                       \
	template<class T>                                                                               \
	void INLINE_TESTER_##MODULE_NAME##_##TEST_NAME(std::ostream& INLINE_TESTER_OUT);                \
	template void INLINE_TESTER_##MODULE_NAME##_##TEST_NAME<int>(std::ostream & INLINE_TESTER_OUT); \
	template<class T>                                                                               \
	void INLINE_TESTER_##MODULE_NAME##_##TEST_NAME(std::ostream& INLINE_TESTER_OUT)

#define IT_IS_TRUE(F)                                                                                      \
	if (!(F)) {                                                                                            \
		INLINE_TESTER_OUT << "Expected true: " << #F << "; file: " << __FILE__ << ":" << __LINE__ << "\n"; \
		exit(1);                                                                                           \
	}

#define IT_IS_FALSE(F)                                                                                      \
	if ((F)) {                                                                                              \
		INLINE_TESTER_OUT << "Expected false: " << #F << "; file: " << __FILE__ << ":" << __LINE__ << "\n"; \
		exit(1);                                                                                            \
	}

#define IT_IS_EQ(F, S)                                                                                               \
	if ((F != S)) {                                                                                                  \
		INLINE_TESTER_OUT << "Expected eq, but: " << #F << " != " << #S << "; file: " << __FILE__ << ":" << __LINE__ \
		                  << "\n";                                                                                   \
		exit(1);                                                                                                     \
	}
