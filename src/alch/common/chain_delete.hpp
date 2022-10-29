#pragma once

#include "containers/hash_map.hpp"
#include "smart_ptr.hpp"
#include "test_framework.hpp"
#include "type_id.hpp"

//
#include <functional>

namespace al {

namespace {

class DeleteChainI {
  public:
	virtual ~DeleteChainI() = default;
};
} // namespace

template<class T>
class DeleteChain: public DeleteChainI {
	friend class ChainDeleter;

  private:
	bool _inChain{};
	SharedPtr<T> _next;
};

class ChainDeleter {
	struct ChainControl {
		SharedPtr<DeleteChainI> chain;
		std::function<void(SharedPtr<DeleteChainI>&)> releaser;
	};

  public:
	template<class T>
	void pushDeleteChain(const SharedPtr<T>& dc) {
		static_assert(std::is_base_of_v<DeleteChainI, T>);

		using BaseType = DeleteChain<T>;

		if (_chains.count(TypeId<T>()) == 0) {
			auto& ch = _chains.insert({TypeId<T>(), {}}).first->second;

			ch.releaser = [this](SharedPtr<DeleteChainI>& chi) {
				auto e = chi.cast<BaseType>();

				chi.reset();

				while (e) {
					if (!e.unique()) {
						pushDeleteChain(e.template cast<T>());
					}
					e = std::move(e->_next);
				}
			};
		}
		auto& ch = _chains[TypeId<T>()].chain;
		dc->_next = ch.template cast<T>();
		ch = dc;
	}

	void release() {
		for (auto& [_, cc] : _chains) {
			cc.releaser(cc.chain);
		}

		for (auto it = _chains.begin(); it != _chains.end();) {
			if (!it->second.chain) {
				auto cp = it++;
				_chains.erase(cp);
			} else {
				++it;
			}
		}
	}

  private:
	HashMap<type_id_t, ChainControl> _chains;
};

} // namespace al

INLINE_TESTER(DeleteChain, Test) {
	using namespace al;
	class DD: public DeleteChain<DD> {
	  public:
		DD(int& i): i(i) {}
		int& i;
		~DD() { i = 0; }
	};

	ChainDeleter cd;

	int i = 5;

	cd.pushDeleteChain(SharedPtr<DD>::make(i));

	IT_IS_TRUE(i == 5);

	cd.release();

	IT_IS_TRUE(i == 0);
}
