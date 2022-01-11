#include <engine/context.hpp>
#include <engine/entity.hpp>
#include <engine/events.hpp>
#include <systems/assets/asset_cache.hpp>
#include <systems/render/camera.hpp>
#include <systems/render/render.hpp>
#include <systems/render/sprite_component.hpp>
#include <systems/scenes/scene.hpp>
#include <systems/transform/transform.hpp>
#include <systems/window/inputs.hpp>
#include <systems/window/window.hpp>
//
#include <memory>

class EditorScene: public Scene {
  public:
	explicit EditorScene(Context& context): Scene(context){};

	void active(bool active) override;

	void onFrame() override;

	Entity* root() override { return _root.get(); };

  private:
	Entity* _selected{};
	// return - if node is deleted
	bool drawNode(Entity& entity, Entity* selected);

  private:
	std::unique_ptr<Entity> _root;
};