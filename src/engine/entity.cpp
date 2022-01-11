#include "entity.hpp"

#include <systems/group/group.hpp>
#include <systems/names/name.hpp>
#include <systems/physics/body.hpp>
#include <systems/physics/collider.hpp>
#include <systems/render/camera.hpp>
#include <systems/render/sprite_component.hpp>
#include <systems/transform/transform.hpp>
//
#include <cereal/cereal.hpp>
#include <common/archive.hpp>
//
#include <unordered_map>

static std::unordered_map<std::string, void (*)(OArchive& oa, Entity& e)> serializeFns = {};
static std::unordered_map<std::string, void (*)(IArchive& oa, Entity& e)> deserializeFns = {};
static std::unordered_map<type_id_t, std::string> idToName = {};

template<>
void Entity::serialize(IArchive& ar) {
	if (!_components.empty()) {
		_components.clear();
	}

	deserializeFns[idToName[TypeId<Transform>()]](ar, *this);

	int count;
	ar >> count;
	while (count-- != 0) {
		std::string name;
		ar >> name;
		deserializeFns[name](ar, *this);
	}
}

template<>
void Entity::serialize(OArchive& ar) {
	serializeFns[idToName[TypeId<Transform>()]](ar, *this);

	ar << static_cast<int>(_components.size());
	// Body must be created before colider
	if (auto body = get<Body>()) {
		const auto& name = idToName[TypeId<Body>()];
		ar << name;
		serializeFns[name](ar, *this);
	}
	for (auto& [id, c] : _components) {
		if (id == TypeId<Body>()) {
			// Body already serialized
			continue;
		}
		const auto& name = idToName[id];
		ar << name;
		serializeFns[name](ar, *this);
	}
}

inline void TransformSerialize(OArchive& oa, Entity& e) {
	auto& t = e.ref<Transform>();
	t.serialize(oa);
}

inline void TransformDeserialize(IArchive& ia, Entity& e) {
	auto& t = e.tr();
	t.serialize(ia);
}

#define ADD_COMPONENT(NAME)                                     \
	idToName.emplace(TypeId<NAME>(), #NAME);                    \
	serializeFns.emplace(#NAME, [](OArchive& oa, Entity& e) {   \
		auto& c = e.ref<NAME>();                                \
		c.serialize(oa);                                        \
	});                                                         \
	deserializeFns.emplace(#NAME, [](IArchive& ia, Entity& e) { \
		auto& c = e.add<NAME>().ref<NAME>();                    \
		c.serialize(ia);                                        \
	});

void Entity::initDefaultSerializers() {
	serializeFns = {
	    {"Transform", &TransformSerialize},
	};
	deserializeFns = {
	    {"Transform", &TransformDeserialize},
	};
	idToName = {
	    {TypeId<Transform>(), "Transform"},
	};

	ADD_COMPONENT(Name);
	ADD_COMPONENT(Body);
	ADD_COMPONENT(Collider);
	ADD_COMPONENT(Camera);
	ADD_COMPONENT(SpriteComponent);
	ADD_COMPONENT(Group);
}

void Entity::registerComponentSerializer(std::string name, type_id_t id, void (*deserializer)(IArchive& oa, Entity& e),
    void (*serializer)(OArchive& oa, Entity& e)) {

	idToName.emplace(id, name);
	serializeFns.emplace(name, serializer);
	deserializeFns.emplace(std::move(name), deserializer);
}