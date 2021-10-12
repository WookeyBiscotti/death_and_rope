#pragma once

#include "cell.hpp"
#include "config.hpp"
#include "vector_utils.hpp"

#include <SFML/Graphics.hpp>
#include <array>
#include <deque>

class WorldCell {
  public:
	void draw(sf::RenderWindow& window);

	void update();

	void shaffle();

	void moveRight();
	void moveLeft();
	void moveUp();
	void moveDown();
	void dec();
	void inc();

	bool rayCast(const sf::Vector2f& start, const sf::Vector2f& end, sf::Vector2f& result);

	CellId getCellId(const sf::Vector2f& p) const;
	CellId getCellId(const sf::Vector2u& p) const;

	sf::Vector2u getIdx(sf::Vector2f p) const;
	sf::Vector2f getVector(const sf::Vector2u& p) const;

	sf::Vector2f getPlayerPosition() const { return _new.position; }

	void ropeInt();

  private:
  private:
	struct Player {
		sf::Vector2f position;
		sf::Vector2f velocity;
	};
	Player _old{};
	Player _new{};

  public:
	struct Rope {
		std::deque<sf::Vector2f> points;
		std::deque<bool> plus;
		void update() {
			if (points.size() < 2) {
				return;
			}
			length = ::length(points[points.size() - 1], points[points.size() - 2]);
		}
		float length;
	} _rope;

	std::array<std::array<CellId, WORLD_CELL_SIZE>, WORLD_CELL_SIZE> _subCell{};
};