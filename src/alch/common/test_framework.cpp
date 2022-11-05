#include "test_framework.hpp"

#include "alch/common/containers/hash_map.hpp"
#include "alch/common/containers/hash_set.hpp"

#include <absl/hash/hash.h>
#include <tabulate/table.hpp>
#include <tabulate/tabulate.hpp>

namespace {
using Registry = al::HashMap<std::string_view /*file*/, al::HashSet<al::TestCaseArgs>>;
} // namespace

namespace std {
template<>
struct hash<al::TestCaseArgs> {
	std::size_t operator()(const al::TestCaseArgs& args) const { return absl::Hash<al::TestCaseArgs>()(args); }
};
} // namespace std

static Registry& testRegisterGet() {
	static Registry reg;

	return reg;
}

int al::registerTestCase(const TestCaseArgs& arg) {
	testRegisterGet()[arg.fileName].insert(arg);
	return 0;
}

// TODO: make PR to tabulate repo with size method
std::size_t rows(tabulate::Table& t) {
	std::size_t s = 0;
	auto it = t.begin();
	while (it != t.end()) {
		++it;
		++s;
	}
	return s;
}

void al::runTests() {
	using namespace tabulate;
	Table result;

	result.add_row({"Builtin tests"});

	const auto& reg = testRegisterGet();

	al::TestContext ctx{std::cout};
	for (const auto& tc : reg) {
		result.add_row({tc.first});
		result[rows(result) - 1].format().font_color(Color::grey).font_style({FontStyle::italic});
		Table tests;
		for (const auto& t : tc.second) {
			t.fn(ctx);
			tests.add_row({t.msg, "Pass"});
		}
		tests[0].format().font_color(Color::green);
		result.add_row({tests});
	}

	result.add_row({"All tests passed"}).format().font_style({FontStyle::bold}).font_align(FontAlign::center);

	result.format().font_align(FontAlign::center);

	result[0].format().font_style({FontStyle::bold}).font_align(FontAlign::center).font_color(Color::blue);
	result[rows(result) - 1]
	    .format()
	    .font_style({FontStyle::bold})
	    .font_align(FontAlign::center)
	    .font_color(Color::green);

	result.print(ctx.out);
}
