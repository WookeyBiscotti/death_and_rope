#include "entity.hpp"

#include "alch/common/type_id.hpp"
#include "alch/engine/component.hpp"
#include "alch/systems/group/group.hpp"
#include "alch/systems/logging/logger.hpp"
#include "alch/systems/names/name.hpp"
#include "alch/systems/physics/body.hpp"
#include "alch/systems/physics/collider.hpp"
#include "alch/systems/render/camera.hpp"
#include "alch/systems/render/sprite_component.hpp"
#include "alch/systems/transform/transform.hpp"
//
#include "alch/common/archive.hpp"

#include <cereal/cereal.hpp>
//
#include <functional>
#include <unordered_map>
#include <vector>

struct SerializerData {
	std::string name;
	type_id_t dependsOn{};
	std::unique_ptr<Component> (*creator)(Entity& ent){};
};

static std::unordered_map<type_id_t, SerializerData> serializerData = {};
static std::unordered_map<std::string, type_id_t> nameToTypeId = {};

void Entity::serialize(OArchive& ar) const {
	auto SD = [this](type_id_t id) -> SerializerData& {
		if (auto found = serializerData.find(id); found == serializerData.end()) {
			LCRIT("Abort serialization. Serializer with id({}) don't exist", id);
			exit(1);
		} else {
			return found->second;
		}
	};

	std::unordered_set<type_id_t> componentInOrder;

	ar(_components.size() + BuiltInCount);
	ar(SD(TypeId<Transform>()).name);
	_transform.serialize(ar);
	componentInOrder.insert(TypeId<Transform>());

	while (componentInOrder.size() != _components.size() + BuiltInCount) {
		const auto orderSize = componentInOrder.size();
		for (const auto& [id, c] : _components) {
			auto& sd = SD(id);
			if (!componentInOrder.contains(id) &&
			    (sd.dependsOn == type_id_t{} || componentInOrder.contains(sd.dependsOn))) {
				componentInOrder.insert(id);
				ar(sd.name);
				c->serialize(ar);
			}
		}
		if (componentInOrder.size() == orderSize) {
			LCRIT("Abort serialization. You have cyclic dependency in serialization");
		}
	}
}

void Entity::deserialize(IArchive& ar) {
	if (!_components.empty()) {
		_components.clear();
	}

	size_t count;
	ar >> count;
	while (count-- != 0) {
		std::string name;
		ar >> name;
		type_id_t id;
		if (auto found = nameToTypeId.find(name); found == nameToTypeId.end()) {
			LCRIT("Abort deserialization: serializer with such name({}) don't registered", name);
			exit(1);
		} else {
			id = found->second;
		}
		if (auto found = serializerData.find(id); found == serializerData.end()) {
			LCRIT("Abort deserialization. Something wrong heppens: serializer with name({}) and id({}) don't exist",
			    name, id);
			exit(1);
		} else {
			const auto& sd = found->second;
			if (!this->get(id)) {
				auto c = sd.creator(*this);
				this->add(id, std::move(c));
			}
			get(id)->deserialize(ar);
		}
	}
}

// template<>
// void Entity::serialize(IArchive& ar) {
// if (!_components.empty()) {
// 	_components.clear();
// }

// deserializeFns[idToName[TypeId<Transform>()]](ar, *this);

// int count;
// ar >> count;
// while (count-- != 0) {
// 	std::string name;
// 	ar >> name;
// 	deserializeFns[name](ar, *this);
// }
// }

// template<>
// void Entity::serialize(OArchive& ar) {
// serializeFns[idToName[TypeId<Transform>()]](ar, *this);

// ar << static_cast<int>(_components.size());
// // Body must be created before colider
// if (auto body = get<Body>()) {
// 	const auto& name = idToName[TypeId<Body>()];
// 	ar << name;
// 	serializeFns[name](ar, *this);
// }
// for (auto& [id, c] : _components) {
// 	if (id == TypeId<Body>()) {
// 		// Body already serialized
// 		continue;
// 	}
// 	const auto& name = idToName[id];
// 	ar << name;
// 	serializeFns[name](ar, *this);
// }
// }

// inline void TransformSerialize(OArchive& oa, Entity& e) {
// 	auto& t = e.ref<Transform>();
// 	t.serialize(oa);
// }

// inline void TransformDeserialize(IArchive& ia, Entity& e) {
// 	auto& t = e.tr();
// 	t.serialize(ia);
// }

// #define ADD_COMPONENT(NAME)                                     \
// 	idToName.emplace(TypeId<NAME>(), #NAME);                    \
// 	serializeFns.emplace(#NAME, [](OArchive& oa, Entity& e) {   \
// 		auto& c = e.ref<NAME>();                                \
// 		c.serialize(oa);                                        \
// 	});                                                         \
// 	deserializeFns.emplace(#NAME, [](IArchive& ia, Entity& e) { \
// 		auto& c = e.add<NAME>().ref<NAME>();                    \
// 		c.serialize(ia);                                        \
// 	});

// void Entity::initDefaultSerializers() {
// 	serializeFns = {
// 	    {"Transform", &TransformSerialize},
// 	};
// 	deserializeFns = {
// 	    {"Transform", &TransformDeserialize},
// 	};
// 	idToName = {
// 	    {TypeId<Transform>(), "Transform"},
// 	};

// 	ADD_COMPONENT(Name);
// 	ADD_COMPONENT(Body);
// 	ADD_COMPONENT(Collider);
// 	ADD_COMPONENT(Camera);
// 	ADD_COMPONENT(SpriteComponent);
// 	ADD_COMPONENT(Group);
// }

// void Entity::registerComponentSerializer(std::string name, type_id_t id, void (*deserializer)(IArchive& oa, Entity&
// e),
//     void (*serializer)(OArchive& oa, Entity& e)) {

// 	idToName.emplace(id, name);
// 	serializeFns.emplace(name, serializer);
// 	deserializeFns.emplace(std::move(name), deserializer);
// }

void Entity::registerComponent(
    type_id_t id, std::unique_ptr<Component> (*creator)(Entity& ent), std::string name, type_id_t dependsOn) {
	if (auto found = serializerData.find(id); found != serializerData.end()) {
		LCRIT("Abort registration: serializer with such id({}) already registered. Registered info: id: {}, name: {}, "
		      "dependsOn: {}. You try register: id: {}, name: {},, dependsOn: {}",
		    id, id, found->second.name, found->second.dependsOn, id, name, dependsOn);
		exit(1);
	}
	if (auto found = nameToTypeId.find(name); found != nameToTypeId.end()) {
		const auto& registeredSer = serializerData[nameToTypeId[name]];
		LCRIT(
		    "Abort registration: serializer with such name({}) already registered. Registered info: id: {}, name: {}, "
		    "dependsOn: {}. You try register: id: {}, name: {}, dependsOn: {}",
		    name, id, registeredSer.name, registeredSer.dependsOn, id, name, dependsOn);
		exit(1);
	}

	SerializerData data;
	data.name = name;
	data.dependsOn = dependsOn;
	data.creator = creator;
}
