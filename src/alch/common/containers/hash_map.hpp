#pragma once

#include <absl/container/flat_hash_map.h>

namespace al {

template<class K, class V>
using HashMap = absl::flat_hash_map<K, V>;

}

template<class Archive, class K, class V>
void serialize(Archive& ar, const al::HashMap<K, V>& hm) {
	size_t size = hm.size();
	ar(size);
	for (const auto& [k, v] : hm) {
		ar(k, v);
	}
}

template<class Archive, class K, class V>
void serialize(const Archive& ar, al::HashMap<K, V>& hm) {
	size_t size;
	ar(size);
	while (size--) {
		K k;
		V v;
		ar(k, v);
	}
}
