#include "test_framework.hpp"

#include "alch/common/containers/hash_map.hpp"
#include "alch/common/containers/hash_set.hpp"
#include "test_app.hpp"

#include <absl/hash/hash.h>
#include <cpp-terminal/window.hpp>

namespace {
using Registry = al::FlatMap<std::string_view /*file*/, al::FlatSet<al::TestCaseArgs>>;
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

void al::runTests() {
	namespace tr = Term;

	const auto& reg = testRegisterGet();

	al::TestContext ctx{std::cout};

	ctx.out << tr::style(tr::Style::BOLD) << tr::color_fg(tr::Color4::BLUE) << "Start test\n";

	for (const auto& tc : reg) {
		ctx.out << tr::style(tr::Style::RESET) << tr::style(tr::Style::ITALIC) << tr::color_fg(tr::Color4::GRAY)
		        << tc.first << tr::style(tr::Style::RESET) << std::endl;
		for (const auto& t : tc.second) {
			t.fn(ctx);
			ctx.out << tr::style(tr::Style::BOLD) << tr::color_fg(tr::Color4::GREEN) << "[Pass]	";
			ctx.out << tr::style(tr::Style::RESET) << tr::color_fg(tr::Color4::WHITE) << t.msg << "\n";
		}
	}

	ctx.out << tr::style(tr::Style::BOLD) << tr::color_fg(tr::Color4::GREEN) << "All tests passed!\n";

	TestApp::get().exit();
	TestApp::get().waitExit();
}

void al::ALCH_IN_FRAME_Impl(std::function<void(Context&)> fn) {
	TestApp::get().push(std::move(fn));
}
