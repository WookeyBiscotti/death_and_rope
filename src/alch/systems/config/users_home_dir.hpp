#pragma once

#include "alch/common/path.hpp"

#if defined(linux) || defined(__APPLE__)
#include <pwd.h>
#include <sys/types.h>
#include <unistd.h>
#else
static_assert(false, "define for other platforms");
#endif

namespace al {

inline Path getHomeDir() {
	const char* homedir;

	if ((homedir = getenv("HOME")) == NULL) {
		homedir = getpwuid(getuid())->pw_dir;
	}

	return homedir;
}

}
