#pragma once

#include "receiver.hpp"

#include <engine/component.hpp>
#include <engine/context.hpp>
#include <engine/entity.hpp>

class ReceiverComponent: public Component, public Receiver {
  public:
	explicit ReceiverComponent(Entity& entity): Component(entity), Receiver(entity.context().systemRef<Broker>()) {}
	virtual ~ReceiverComponent() = default;
};