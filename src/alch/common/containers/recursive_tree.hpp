#pragma once

#include "alch/common/containers/hash_map.hpp"
#include "alch/common/containers/inline_vector.hpp"
#include "alch/common/containers/map.hpp"
#include "alch/common/containers/span.hpp"
#include "alch/common/containers/string.hpp"
#include "alch/common/containers/vector.hpp"
#include "alch/common/variant.hpp"

namespace al {

template<class K, class V>
class RecursiveTree {
  public:
	using Map = NodeHashMap<K, RecursiveTree>;
	using Value = Variant<V, Map>;
	using Path = InlinedVector<K, 4>;
	using PathSpan = Span<K>;

	bool isEmpty(const Path& path) const { return isEmpty(MakeSpan(path)); }

	bool isEmpty(const PathSpan& path) const {
		if (path.empty()) {
			return false;
		}
		auto found = _values.find(path.front());
		if(found != _values.end()) {
			if(path.size() == 1) {
				return true;
			} else {
				const auto& v = found->
			}
		}
	}

	template<class Archive>
	void serialize(Archive& ar) {
		ar(_values);
	}

  private:
	Value _values;
};

} // namespace al
