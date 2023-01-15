#pragma once

#include <absl/container/flat_hash_map.h>
#include <absl/container/node_hash_map.h>
//
#include <cereal/cereal.hpp>
#include <cereal/archives/json.hpp>


namespace al {

template<class K, class V>
using FlatMap = absl::flat_hash_map<K, V>;

template<class K, class V>
using NodeMap = absl::node_hash_map<K, V>;

template<class Archive, class K, class V>
void save(Archive& ar, const al::FlatMap<K, V>& hm) {
	size_t size = hm.size();
	ar(size);
	for (const auto& [k, v] : hm) {
		serialize(ar, k, cereal::make_nvp(k, v));
	}
}

template<class Archive, class K, class V>
void load(const Archive& ar, al::FlatMap<K, V>& hm) {
	size_t size;
	ar(size);
	while (size--) {
		K k;
		V v;
		serialize(ar, k, v);
	}
}

template<class Archive, class K, class V>
void save(Archive& ar, const al::NodeMap<K, V>& hm) {
	size_t size = hm.size();
	ar(size);
	for (const auto& [k, v] : hm) {
		ar(k, cereal::make_nvp(k, v));
	}
}

template<class Archive, class K, class V>
void load(const Archive& ar, al::NodeMap<K, V>& hm) {
	size_t size;
	ar(size);
	while (size--) {
		K k;
		V v;
		ar(k, v);
	}
}

} // namespace al
