#pragma once

#include <SFML/Graphics/View.hpp>
#include <systems/scenes/scene.hpp>
#include <world.hpp>

class WorldEditor: public Scene {
  public:
	explicit WorldEditor(Context& context);

	void onFrame() override;

	void active(bool active) override;

  private:
	void paintPoint(const Vector2f& p);

  private:
	bool _showLoadDialog = false;
	std::vector<std::string> _worldsList;

	bool _showSaveDialog = false;
	std::string _saveName;

	int _currentTileType = 0;
	sf::View _view;
	World _world;
};