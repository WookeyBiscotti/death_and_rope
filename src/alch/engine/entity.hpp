#pragma once

#include <common/archive.hpp>
//
#include "component.hpp"
#include "context.hpp"

#include <common/prod_build_utils.hpp>
#include <common/type_id.hpp>
#include <systems/broker/broker.hpp>
#include <systems/logging/logger.hpp>
#include <systems/transform/transform.hpp>
//
#include <cassert>
#include <memory>
#include <unordered_map>
#include <utility>

class Context;

class Entity final: public Sender, public Receiver {
  public:
	explicit Entity(Context& context):
	    Sender(context.systemRef<Broker>()), Receiver(context.systemRef<Broker>()), _context(context),
	    _transform(*this) {}

	Entity(const Entity&) = delete;
	Entity(Entity&&) = delete;
	Entity& operator=(const Entity&) = delete;
	Entity& operator=(const Entity&&) = delete;

	Context& context() { return _context; }

	template<class C>
	auto& add(std::unique_ptr<C>&& component) {
		static_assert(TypeId<C>() != TypeId<Transform>());

		add(TypeId<C>(), std::move(component));

		return *this;
	}
	template<class C, class... Args>
	auto& add(Args&&... args) {
		static_assert(TypeId<C>() != TypeId<Transform>());

		auto c = std::make_unique<C>(*this, std::forward<Args>(args)...);
		_components.emplace(TypeId<C>(), std::move(c));

		return *this;
	}

	template<class C>
	bool remove() {
		static_assert(TypeId<C>() != TypeId<Transform>());

		return _components.erase(TypeId<C>()) != 0;
	}

	template<class C>
	C* get() {
		static_assert(TypeId<C>() != TypeId<Transform>());

		if (auto found = _components.find(TypeId<C>()); found != _components.end()) {
			return static_cast<C*>(found->second.get());
		}

		return nullptr;
	}
	template<>
	Transform* get<Transform>() {
		return &_transform;
	}

	template<class C>
	C& ref() {
		IF_NOT_PROD_BUILD(                                      //
		    auto c = get<C>();                                  //
		    if (!c) {                                           //
			    LERR("No such component {}", typeid(C).name()); //
			    assert(false);                                  //
		    }                                                   //
		    return *c;);

		return *static_cast<C*>(_components.find(TypeId<C>())->second.get());
	}
	template<>
	Transform& ref<Transform>() {
		return _transform;
	}

	auto& transform() { return _transform; }
	auto& tr() { return transform(); }

	void serialize(OArchive& ar) const;
	void deserialize(IArchive& ar);

	// static void initDefaultSerializers();

	// template<class C>
	// static void registerComponentSerializer(std::string name, void (*deserializer)(IArchive& oa, Entity& e),
	//     void (*serializer)(OArchive& oa, Entity& e)) {
	// 	registerComponentSerializer(std::move(name), TypeId<C>(), deserializer, serializer);
	// }
	// static void registerComponentSerializer(std::string name, type_id_t id,
	//     void (*deserializer)(IArchive& oa, Entity& e), void (*serializer)(OArchive& oa, Entity& e));

	template<class C>
	static void registerComponent(std::string name, type_id_t dependsOn = {}) {
		bool registerComponent(
		    TypeId<C>(), [](Entity& ent) { return std::make_unique<C>(ent); }, std::move(name), dependsOn);
	}

	static void registerComponent(
	    type_id_t id, std::unique_ptr<Component> (*creator)(Entity& ent), std::string name, type_id_t dependsOn);

  private:
	// unsafe
	auto& add(type_id_t id, std::unique_ptr<Component>&& component) {
		_components.emplace(id, std::move(component));

		return *this;
	}

	// unsafe
	Component* get(type_id_t id) {
		if (id == TypeId<Transform>()) {
			return &_transform;
		} else {
			if (auto it = _components.find(id); it != _components.end()) {
				return it->second.get();
			} else {
				return nullptr;
			}
		}
	}

  private:
	Context& _context;

	static constexpr size_t BuiltInCount = 1; 
	Transform _transform;

	std::unordered_map<type_id_t, std::unique_ptr<Component>> _components;
};
