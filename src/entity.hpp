#pragma once

#include <ctti/type_id.hpp>
#include <unordered_map>

#include "component.hpp"

class Entity
{
public:
    template<class C>
    bool add(std::shared_ptr<C> component)
    {
        return _components.emplace(ctti::type_id<C>(), std::move(component)).second;
    }

    template<class C>
    bool remove()
    {
        return _components.erase(ctti::type_id<C>()) != 0;
    }

    template<class C>
    C* get()
    {
        if (auto found = _components.find(ctti::type_id<C>()); found != _components.end()) {
            return static_cast<C*>(found->second.get());
        }

        return nullptr;
    }

    template<class C>
    std::shared_ptr<C> getShared()
    {
        if (auto found = _components.find(ctti::type_id<C>()); found != _components.end()) {
            return found.second;
        }

        return nullptr;
    }

private:
    std::unordered_map<ctti::type_id_t, std::shared_ptr<Component>> _components;
};