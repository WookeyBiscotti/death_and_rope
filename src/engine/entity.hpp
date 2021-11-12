#pragma once

#include "component.hpp"

#include <cassert>
#include <common/prod_build_utils.hpp>
#include <common/type_id.hpp>
#include <systems/logging/logger.hpp>
//
#include <memory>
#include <unordered_map>
#include <utility>

class Context;

class Entity final {
  public:
	explicit Entity(Context& context): _context(context) {}

	Entity(Entity&&) = delete;
	Entity(const Entity&) = delete;
	Entity& operator=(const Entity&) = delete;
	Entity& operator=(const Entity&&) = delete;

	Context& context() { return _context; }

	template<class C>
	auto& add(std::unique_ptr<C>&& component) {
		_components.emplace(TypeId<C>(), std::move(component));

		return *this;
	}

	template<class C, class... Args>
	auto& add(Args&&... args) {
		auto c = std::make_unique<C>(*this, std::forward<Args>(args)...);
		_components.emplace(TypeId<C>(), std::move(c));

		return *this;
	}

	template<class C>
	bool remove() {
		return _components.erase(TypeId<C>()) != 0;
	}

	template<class C>
	C* get() {
		if (auto found = _components.find(TypeId<C>()); found != _components.end()) {
			return static_cast<C*>(found->second.get());
		}

		return nullptr;
	}

	template<class C>
	C& ref() {
		IF_NOT_PROD_BUILD(     //
		    auto c = get<C>(); //
		    if (!c) {
			    LERR("No such component {}", typeid(C).name());
			    assert(false);
		    } //
		    return *c;);

		IF_PROD_BUILD(return *get<C>(););
	}

  private:
	Context& _context;

	std::unordered_map<type_id_t, std::unique_ptr<Component>> _components;
};