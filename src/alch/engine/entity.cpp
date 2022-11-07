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


using namespace al;

struct SerializerData {
	std::string name;
	std::vector<type_id_t> dependsOn{};
	SharedPtr<Component> (*creator)(Entity& ent){};
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

	ar(SD(TypeId<Parent>()).name);
	_parent.serialize(ar);

	componentInOrder.insert(TypeId<Parent>());

	while (componentInOrder.size() != _components.size() + BuiltInCount) {
		const auto orderSize = componentInOrder.size();
		for (const auto& [id, c] : _components) {
			auto& sd = SD(id);
			if (!componentInOrder.contains(id)) {
				bool canSerialize = true;
				for (const auto dpOn : sd.dependsOn) {
					if (!componentInOrder.contains(dpOn)) {
						break;
						canSerialize = false;
					}
				}
				if (!canSerialize) {
					continue;
				}
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

void Entity::registerComponent(type_id_t id, SharedPtr<Component> (*creator)(Entity& ent), std::string name,
    std::vector<type_id_t> dependsOn) {
	if (auto found = serializerData.find(id); found != serializerData.end()) {
		LCRIT("Abort registration: serializer with such id({}) already registered. Registered info: id: {}, name: {}. "
		      "You try register: id: {}, name: {}",
		    id, id, found->second.name, id, name);
		exit(1);
	}
	if (auto found = nameToTypeId.find(name); found != nameToTypeId.end()) {
		const auto& registeredSer = serializerData[nameToTypeId[name]];
		LCRIT("Abort registration: serializer with such name({}) already registered. Registered info: id: {}, name: "
		      "{}. You try register: id: {}, name: {}.",
		    name, id, registeredSer.name, id, name);
		exit(1);
	}

	SerializerData data;
	data.name = name;
	data.dependsOn = std::move(dependsOn);
	data.creator = creator;

	serializerData.emplace(id, std::move(data));
	nameToTypeId.emplace(std::move(name), id);
}
