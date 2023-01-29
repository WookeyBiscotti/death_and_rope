#include "ui_choise.hpp"

#include "alch/systems/assets/asset_cache.hpp"
#include "alch/systems/ui/ui_system.hpp"
#include "alch/systems/ui/widgets/ui_button.hpp"
#include "alch/systems/ui/widgets/ui_window.hpp"
//
#include <SFML/Graphics/ConvexShape.hpp>

using namespace al;

UIChoise::UIChoise(Context& context, WeakPtr<UIElement> parent, const std::vector<String>& choses, std::size_t idx):
    UIElement(context, parent), _choices(choses), _idx(idx) {
	if (!_font) {
		_font = context.systemRef<AssetCache>().font(style<StyleName::FONT, String>());
	}
	maxSize({maxSize().x, style<StyleName::CHOISE_HEIGHT, float>()});
}

void UIChoise::draw(sf::RenderTarget& target) {
	using enum StyleName;

	sf::RectangleShape rs;
	rs.setSize(_size);
	rs.setPosition(toWorldCoords(_position));

	rs.setFillColor(style<EDIT_PLACE_COLOR, Color>());
	rs.setOutlineColor(style<BORDER_COLOR, Color>());
	rs.setOutlineThickness(-style<BORDER_THICKNESS, float>());

	target.draw(rs);
	target.draw(_text);

	const auto h = _size.y;
	const auto w = std::min(_size.x, h);
	sf::ConvexShape c;
	c.setPointCount(3);

	auto gp = toWorldCoords(position());
	c.setPoint(0, gp + Vector2f{_size.x, h * 0.5f});
	c.setPoint(1, c.getPoint(0) + Vector2f{0, h * 0.5f});
	c.setPoint(2, c.getPoint(1) - Vector2f{w * 0.5f, 0});
	auto color = style<FLAT_COLOR, Color>();
	color.a = 100;
	c.setFillColor(color);
	c.setOutlineThickness(-style<BORDER_THICKNESS, float>());
	c.setOutlineColor(style<BORDER_COLOR, Color>());

	target.draw(c);
}

void UIChoise::onSizeChange() {
	if (_font) {
		_text.setFont(_font->sf());
	}

	if (!_choices.empty()) {
		_idx = std::min(_idx, _choices.size() - 1);
		const auto& content = _choices[_idx];

		_text.setString(sf::String::fromUtf8(content.begin(), content.end()));
		_text.setCharacterSize(style<StyleName::TEXT_SIZE, float>());
		_text.setFillColor(style<StyleName::TEXT_COLOR, Color>());
		if (style<StyleName::TEXT_BOLD, bool>()) {
			_text.setStyle(sf::Text::Bold);
		}

		auto gb = _text.getGlobalBounds();
		auto gp = toWorldCoords(position());

		_text.setOrigin(0, gb.height / 2);
		_text.setPosition(gp + 0.5f * Vector2f{0, size().y});
	}
}

void UIChoise::onPositionChange() {
	auto gb = _text.getGlobalBounds();
	auto gp = toWorldCoords(position());

	_text.setOrigin(0, gb.height / 2);
	_text.setPosition(gp + 0.5f * Vector2f{0, size().y});
}

bool UIChoise::onMouseWheel(const UIMouseWheel& e) {
	const auto dir = e.event.delta > 0 ? -1 : 1;
	if (dir < 0) {
		if (_idx > 0) {
			_idx--;
			onSizeChange();
		}
	} else {
		if (_choices.size() > 1 && _idx <= _choices.size() - 2) {
			_idx++;
			onSizeChange();
		}
	}

	return true;
}

void UIChoise::onPressed(const UIMouseButtonPressed&) {
	if (_chosesUI) {
		system().popout()->remove(_chosesUI);
	}
	_chosesUI = system().popout()->create<UIWindow>("", true);
	_chosesUIDR = {};
	_chosesUI->size({_size.x, _size.y * _choices.size()});
	auto gp = toWorldCoords(position());
	_chosesUI->position(gp);
	_chosesUI->layout(UIElement::VERICAL);
	size_t idx = 0;
	for (const auto& c : _choices) {
		auto b = _chosesUI->create<UIButton>(c);
		subscribe<UIButtonPressed>(b, [=](const UIButtonPressed&) {
			_idx = idx;
			onSizeChange();
			system().popout()->remove(_chosesUI);
			_chosesUI = {};
			_chosesUIDR = {};
			send(UIChoiseOnChoise{c, idx});
		});
		idx++;
		subscribe<UIElementOnMouseWheel>(b, [=](const UIElementOnMouseWheel& e) {
			const auto dir = e.e.event.delta > 0 ? -1 : 1;
			_chosesUIDR += {0, -10 * e.e.event.delta};
			_chosesUIDR.y = std::max(0.0f, _chosesUIDR.y);
			_chosesUIDR.y = std::min(_size.y * (_choices.size() - 1), _chosesUIDR.y);
			_chosesUI->position(globalPosition() - _chosesUIDR);
		});
	}
	subscribe<UIElementGlobalPositionChange>(this, [=](const UIElementGlobalPositionChange&) {
		if (this->_chosesUI) {
			this->_chosesUI->position(globalPosition() - _chosesUIDR);
		}
	});

	subscribe<UIElementOnUnhovered>(_chosesUI, [&](const UIElementOnUnhovered&) {
		system().popout()->remove(_chosesUI);
		_chosesUI = {};
		_chosesUIDR = {};
	});
}

UIChoise::~UIChoise() {
	if (_chosesUI) {
		system().popout()->remove(_chosesUI);
	}
}
