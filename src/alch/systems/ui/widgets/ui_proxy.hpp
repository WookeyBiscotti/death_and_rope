#pragma once

#include "alch/common/archive.hpp"
#include "alch/common/bitset.hpp"
#include "alch/common/optional.hpp"
#include "alch/common/rect.hpp"
#include "alch/common/smart_ptr.hpp"
#include "alch/common/vector2.hpp"
#include "alch/engine/component.hpp"
#include "alch/systems/broker/transmitter.hpp"
#include "alch/systems/logging/logger.hpp"
#include "alch/systems/ui/ui_element.hpp"
//

namespace al {

class UISystem;
class Context;

class UIProxy: public UIElement {
  public:
	UIProxy(Context& context, WeakPtr<UIElement> parent);

	Layout layout() const override;
	void layout(Layout layout) override;

	GravityH gravityH() const override;
	void gravityH(GravityH g) override;

	GravityV gravityV() const override;
	void gravityV(GravityV g) override;

	virtual void remove(UIElement* element) override;

	virtual void removeAll() override;

	virtual void draw(sf::RenderTarget& target) override;

	const std::vector<SharedPtr<UIElement>>& childs() const override;

	void distanceBetweenChildren(UIUnit distanceBetweenChildren) override;
	UIUnit distanceBetweenChildren() const override;

	void indentLeft(UIUnit indentLeft) override;
	UIUnit indentLeft() const override;

	void indentRight(UIUnit indentRight) override;
	UIUnit indentRight() const override;

	void indentTop(UIUnit indentTop) override;
	UIUnit indentTop() const override;

	void indentBot(UIUnit indentBottom) override;
	UIUnit indentBot() const override;

	void add(SharedPtr<UIElement> element) override;

  protected:
	UIElement* _proxy{};
};

} // namespace al
