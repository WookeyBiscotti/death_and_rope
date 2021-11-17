#pragma once

#include "sender.hpp"

#include <engine/component.hpp>
#include <engine/context.hpp>
#include <engine/entity.hpp>

class SenderComponent: public Component, public Sender {
  public:
	explicit SenderComponent(Entity& entity): Component(entity), Sender(entity.context().systemRef<Broker>()) {}
	virtual ~SenderComponent() = default;
};