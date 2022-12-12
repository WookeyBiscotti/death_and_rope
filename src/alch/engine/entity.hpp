#pragma once

#include "alch/common/archive.hpp"
//
#include "alch/common/containers/hash_map.hpp"
#include "alch/common/prod_build_utils.hpp"
#include "alch/common/smart_ptr.hpp"
#include "alch/common/type_id.hpp"
#include "alch/systems/broker/broker.hpp"
#include "alch/systems/group/parent.hpp"
#include "alch/systems/logging/logger.hpp"
#include "alch/systems/transform/transform.hpp"
#include "component.hpp"
#include "context.hpp"
//
#include <cassert>
#include <memory>
#include <utility>

namespace al {

class Context;
class RootEntity;

class Entity: public Transmitter, public EnableSharedFromThis<Entity> {
	friend class RootEntity;

  public:
	explicit Entity(Context& context):
	    Transmitter(context.systemRef<Broker>()), _context(context), _transform(*this), _parent(*this) {}

	Entity(const Entity&) = delete;
	Entity(Entity&&) = delete;
	Entity& operator=(const Entity&) = delete;
	Entity& operator=(const Entity&&) = delete;

	~Entity() {}

	Context& context() { return _context; }

	template<class C>
	auto& addChain(std::unique_ptr<C>&& component) {
		static_assert(TypeId<C>() != TypeId<Transform>());

		add(TypeId<C>(), std::move(component));

		return *this;
	}
	template<class C, class... Args>
	auto& addChain(Args&&... args) {
		static_assert(TypeId<C>() != TypeId<Transform>());

		auto c = SharedPtr<C>::make(*this, std::forward<Args>(args)...);
		_components.emplace(TypeId<C>(), std::move(c));

		return *this;
	}

	template<class C, class... Args>
	C& add(Args&&... args) {
		static_assert(TypeId<C>() != TypeId<Transform>());
		auto c = SharedPtr<C>::make(*this, std::forward<Args>(args)...);
		auto& ret = *c;
		_components.emplace(TypeId<C>(), std::move(c));

		return ret;
	}

	template<class C>
	bool remove() {
		static_assert(TypeId<C>() != TypeId<Transform>());

		return _components.erase(TypeId<C>()) != 0;
	}

	void clear() { _components.clear(); }

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
	template<>
	Parent* get<Parent>() {
		return &_parent;
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
	template<>
	Parent& ref<Parent>() {
		return _parent;
	}

	auto& transform() { return _transform; }
	auto& tr() { return transform(); }

	auto& parent() { return _parent; }
	void parent(WeakPtr<Entity> parent) { return _parent.parent(parent); }

	void serialize(OArchive& ar) const;
	void deserialize(IArchive& ar);

	template<class C>
	static void registerComponent(Context& context) {
		Entity ent(context);
		auto c = C(ent);
		registerComponent(
		    TypeId<C>(), [](Entity& ent) -> SharedPtr<Component> { return SharedPtr<C>::make(ent); },
		    std::string(c.cName()), c.dependsOn());
	}

  private:
	static void registerComponent(type_id_t id, SharedPtr<Component> (*creator)(Entity& ent), std::string name,
	    std::vector<type_id_t> dependsOn);

	// unsafe
	auto& add(type_id_t id, SharedPtr<Component>&& component) {
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

	static constexpr size_t BuiltInCount = 2;
	Transform _transform;
	Parent _parent;

	HashMap<type_id_t, SharedPtr<Component>> _components;
};

} // namespace al
