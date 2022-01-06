#pragma once

#define DEFINE_SETER_GETER(Name, Member)                    \
	const decltype(Member)& Name() const { return Member; } \
	void Name(const decltype(Member)& other) const { Member = other; }

#define DEFINE_GETER(Name, Member) \
	const decltype(Member)& Name() const { return Member; }
