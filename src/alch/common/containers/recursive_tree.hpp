#pragma once
#include <cereal/archives/binary.hpp>
#include <cereal/archives/json.hpp>
#include <cereal/cereal.hpp>
#include <cereal/specialize.hpp>
#include <cereal/types/variant.hpp>
//
#include "alch/common/containers/hash_map.hpp"
#include "alch/common/containers/inline_vector.hpp"
#include "alch/common/containers/map.hpp"
#include "alch/common/containers/recursive_tree.hpp"
#include "alch/common/containers/span.hpp"
#include "alch/common/containers/string.hpp"
#include "alch/common/containers/vector.hpp"
#include "alch/common/variant.hpp"
//

namespace al {

template<class K, class V>
class RecursiveTreeNode {
  public:
	using Map = NodeMap<K, RecursiveTreeNode>;
	using Path = InlinedVector<K, 4>;
	using PathSpan = Span<const K>;

	void operator()(const V& v) { _value = v; }

	const V& operator()() const { return _value; }
	V& operator()() { return _value; }

	V& operator[](const K& path) { return (*this)[MakeSpan(&path, 1)]; }
	V& operator[](const Path& path) { return (*this)[MakeSpan(path)]; }
	V& operator[](PathSpan path) {
		if (path.empty()) {
			return _value;
		}
		return _childs[path.front()][path.subspan(1)];
	}

	const V* find(const Path& path) const { return find(MakeSpan(path)); }
	const V* find(const K& path) const { return find(MakeSpan(&path, 1)); }
	const V* find(PathSpan path) const {
		if (path.empty()) {
			return &_value;
		}
		auto found = _childs.find(path.front());
		if (found != _childs.end()) {
			return found->second.find(path.subspan(1));
		}

		return nullptr;
	}

	template<class Archive>
	void save(Archive& ar) const {
		ar(_value, _childs);
	}
	template<class Archive>
	void load(Archive& ar) {
		ar(_value, _childs);
	}

  private:
	V _value;
	Map _childs;
};

} // namespace al
