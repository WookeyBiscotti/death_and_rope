#pragma once

#if defined(PROD_BUILD)

#define IF_PROD_BUILD(EXEC) \
	{ EXEC; }

#define NONPROD_ASSERT(COND) ;

#else

#define IF_PROD_BUILD(EXEC)

#include <cassert>
#define NONPROD_ASSERT(COND) assert((COND));

#endif

#if !defined(PROD_BUILD)

#define IF_NOT_PROD_BUILD(EXEC) EXEC;
constexpr bool IS_NOT_PROD_BUILD = true;
#define NOT_PROD_BUILD

#else

#define IF_NOT_PROD_BUILD(EXEC)
#define IS_NOT_PROD_BUILD false;

#endif
