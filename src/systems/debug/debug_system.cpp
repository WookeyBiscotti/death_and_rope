#include "debug_system.hpp"

#if !defined(PROD_BUILD)

#include <engine/context.hpp>
#include <engine/events.hpp>
#include <systems/assets/asset_cache.hpp>
#include <systems/logging/logger.hpp>
#include <systems/physics/physics.hpp>
#include <systems/render/drawable.hpp>
#include <systems/render/render.hpp>
#include <systems/window/events.hpp>
#include <systems/window/inputs.hpp>
#include <systems/window/window.hpp>
//
#include <SFML/Graphics.hpp>
#include <box2d/b2_draw.h>
#include <imgui.h>

class Box2dDebugDraw: public b2Draw {
  public:
	Box2dDebugDraw(RenderTarget& target): _target(target) {}
	void DrawPolygon(const b2Vec2* vertices, int32 vertexCount, const b2Color& color) override {
		std::vector<sf::Vertex> v;
		for (int i = 0; i != vertexCount; ++i) {
			v.push_back(sf::Vertex({vertices[i].x, vertices[i].y},
			    {uint8_t(color.r * 255), uint8_t(color.g * 255), uint8_t(color.b * 255)}));
		}
		v.push_back(sf::Vertex({vertices[0].x, vertices[0].y},
		    {uint8_t(color.r * 255), uint8_t(color.g * 255), uint8_t(color.b * 255)}));
		_target.draw(v.data(), v.size(), sf::LineStrip);
	}

	/// Draw a solid closed polygon provided in CCW order.
	void DrawSolidPolygon(const b2Vec2* vertices, int32 vertexCount, const b2Color& color) override {
		DrawPolygon(vertices, vertexCount, color);
	};

	/// Draw a circle.
	void DrawCircle(const b2Vec2& center, float radius, const b2Color& color) override {
		sf::CircleShape s(radius);
		s.setOutlineColor({uint8_t(color.r * 255), uint8_t(color.g * 255), uint8_t(color.b * 255)});
		s.setOrigin(radius, radius);
		_target.draw(s);
	}

	/// Draw a solid circle.
	void DrawSolidCircle(const b2Vec2& center, float radius, const b2Vec2& axis, const b2Color& color) override {
		sf::CircleShape s(radius);
		s.setFillColor({uint8_t(color.r * 255), uint8_t(color.g * 255), uint8_t(color.b * 255)});
		s.setOrigin(radius, radius);
		_target.draw(s);
	}

	/// Draw a line segment.
	void DrawSegment(const b2Vec2& p1, const b2Vec2& p2, const b2Color& color) override {
		sf::Vertex v[2] = {
		    sf::Vertex({p1.x, p1.y}, {uint8_t(color.r * 255), uint8_t(color.g * 255), uint8_t(color.b * 255)}),
		    sf::Vertex({p2.x, p2.y}, {uint8_t(color.r * 255), uint8_t(color.g * 255), uint8_t(color.b * 255)}),
		};
		_target.draw(v, 2, sf::Lines);
	}

	/// Draw a transform. Choose your own length scale.
	/// @param xf a transform.
	void DrawTransform(const b2Transform& xf) override {}

	/// Draw a point.
	void DrawPoint(const b2Vec2& p, float size, const b2Color& color) override {
		auto v = sf::Vertex({p.x, p.y}, {uint8_t(color.r * 255), uint8_t(color.g * 255), uint8_t(color.b * 255)});
		_target.draw(&v, 1, sf::Points);
	}

  public:
	RenderTarget& _target;
};

DebugSystem::DebugSystem(Context& context): Receiver(context.systemRef<Broker>()), _context(context) {
	_font = context.systemRef<AssetCache>().font("default");

	_text.setFont(_font->sf());
	_text.setCharacterSize(40);
	_text.setScale(0.5, 0.5);

	subscribe<WindowEvent>([this](const WindowEvent& e) {
		if (e.event.type == sf::Event::KeyPressed && e.event.key.code == Keyboard::F12) {
			_isLogShown = !_isLogShown;
		}
	});

	subscribe<EngineOnFrameStart>([this](const EngineOnFrameStart& e) { _lastFps = e.lastFps; });

	subscribe<EngineOnFramePreRender>([this](const EngineOnFramePreRender& e) {
		if (_isLogShown) {
			auto io = ImGui::GetIO();
			const auto W = io.DisplaySize.x;
			const auto H = io.DisplaySize.y;
			ImGui::SetNextWindowSize({W, H / 2});
			ImGui::SetNextWindowPos({0, H / 2});
			ImGui::SetNextWindowBgAlpha(0.5f);
			ImGui::Begin("Logs(F12 to open/close)", nullptr,
			    ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse |
			        ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoBringToFrontOnFocus);
			for (const auto& l : _context.systemRef<Logger>().logs()) {
				ImGui::TextUnformatted(l.c_str());
			}
			ImGui::End();
		}
	});

	subscribe<EngineOnFramePostRender>([this](const EngineOnFramePostRender& e) {
		if (_frameCounter++ == 60) {
			_frameCounter = 0;
			_text.setString(std::to_string(int(_lastFps)));
		}
		auto& window = _context.systemRef<Window>().window();
		auto save = window.getView();
		window.setView(window.getDefaultView());
		window.draw(_text);
		window.setView(save);

		if (auto p = _context.system<Physics>()) {
			p->internalWorld().DebugDraw();
		}
	});

	if (auto p = _context.system<Physics>()) {
		if (auto r = _context.system<Render>()) {
			_debugBox2dDraw = std::make_shared<Box2dDebugDraw>(r->target());
			p->internalWorld().SetDebugDraw(_debugBox2dDraw.get());
			_debugBox2dDraw->SetFlags(b2Draw::e_shapeBit | b2Draw::e_aabbBit);
		}
	}
}

#endif