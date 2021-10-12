#include "world_cell.hpp"

#include "vector_utils.hpp"

#include <cmath>
#include <iostream>

void WorldCell::draw(sf::RenderWindow& window) {
	sf::RectangleShape rs;
	rs.setFillColor(sf::Color::Green);
	rs.setSize({SUBCELL_SIZE, SUBCELL_SIZE});
	for (auto x = 0u; x != _subCell.size(); ++x) {
		for (auto y = 0u; y != _subCell.front().size(); ++y) {
			rs.setPosition({1.0f * x * SUBCELL_SIZE, 1.0f * y * SUBCELL_SIZE});
			if (_subCell[x][y] != 0) {
				window.draw(rs);
			}
		}
	}

	sf::CircleShape cs;
	cs.setPosition(_new.position);
	cs.setFillColor(sf::Color::White);
	cs.setRadius(SUBCELL_SIZE / 4);
	cs.setOrigin(cs.getRadius(), cs.getRadius());
	window.draw(cs);
}

void WorldCell::shaffle() {
	for (auto x = 0u; x != _subCell.size(); ++x) {
		for (auto y = 0u; y != _subCell.front().size(); ++y) {
			_subCell[x][y] = rand() % 4 == 0;
		}
	}
	_subCell[0][0] = 0;
	_old = _new = {};
}

void WorldCell::moveRight() {
	_new.velocity.x += 0.1f;
}

void WorldCell::moveLeft() {
	_new.velocity.x -= 0.1f;
}

void WorldCell::moveUp() {
	_new.velocity.y -= 0.1f;
}
void WorldCell::moveDown() {
	_new.velocity.y += 0.1f;
}

void WorldCell::dec() {
	_rope.lenght += 1.0f;
}

void WorldCell::inc() {
	_rope.lenght = std::max(1.0f, _rope.lenght - 1.0f);
}

void WorldCell::ropeInt() {
	if (_rope.points.size() < 2) {
		return;
	}
	if (_rope.points.size() > 2) {
		auto a = angle(_rope.points[_rope.points.size() - 1], _rope.points[_rope.points.size() - 2],
		    _rope.points[_rope.points.size() - 2], _rope.points[_rope.points.size() - 3]);
		if (a > 0 && _rope.plus[_rope.plus.size() - 1] || a < 0 && !_rope.plus[_rope.plus.size() - 1]) {
			_rope.plus.pop_back();
			_rope.points.erase(_rope.points.end() - 2);
		}
	}
	sf::Vector2f res;
	if (rayCast(_rope.points[_rope.points.size() - 1], _rope.points[_rope.points.size() - 2], res)) {
		_rope.points.insert(_rope.points.end() - 1, res);
		_rope.plus.push_back(angle(_rope.points[_rope.points.size() - 1], _rope.points[_rope.points.size() - 2],
		                         _rope.points[_rope.points.size() - 2], _rope.points[_rope.points.size() - 3]) > 0);
	}

	_rope.update();
}

void WorldCell::update() {
	auto old = _new;
	_new.position += _new.velocity;
	_new.velocity += {0, 0.15f};

	const int x = _new.position.x / SUBCELL_SIZE;
	const int y = _new.position.y / SUBCELL_SIZE;
	const int oldX = _old.position.x / SUBCELL_SIZE;
	const int oldY = _old.position.y / SUBCELL_SIZE;

	if (_subCell[x][y] != 0) {
		_new.position = _old.position;
		if (oldY != y) {
			_new.velocity.y = 0;
		} else if (oldX != x) {
			_new.velocity.x = 0;
		}
	} else {
		_new.velocity *= 0.9999f;
	}
	_old = old;

	if (_rope.is) {
		auto l = length(_rope.p - _new.position);
		auto save = _new;
		auto n = mod(_rope.p - _new.position);
		auto a = athwart(n);
		_new.position += n * (l - _rope.lenght);
		auto res = _new.position;
		if (rayCast(save.position, _new.position, res)) {
			_new.position = save.position;
			_rope.lenght = l;
		}
		_new.velocity = a * cross(_new.velocity, a);
	}
}

CellId WorldCell::getCellId(const sf::Vector2f& p) const {
	if (p.x < 0 && p.x >= WORLD_CELL_SIZE * SUBCELL_SIZE) {
		return 0;
	}
	if (p.y < 0 && p.y >= WORLD_CELL_SIZE * SUBCELL_SIZE) {
		return 0;
	}

	const size_t x = _new.position.x / SUBCELL_SIZE;
	const size_t y = _new.position.y / SUBCELL_SIZE;

	return _subCell[x][y];
}

CellId WorldCell::getCellId(const sf::Vector2u& p) const {
	return _subCell[p.x][p.y];
}

sf::Vector2u WorldCell::getIdx(sf::Vector2f p) const {
	if (p.x < 0) {
		p.x = 0;
	} else if (p.x >= WORLD_CELL_SIZE * SUBCELL_SIZE) {
		p.x = WORLD_CELL_SIZE * SUBCELL_SIZE;
	}
	if (p.y < 0) {
		p.y = 0;
	} else if (p.y >= WORLD_CELL_SIZE * SUBCELL_SIZE) {
		p.y = WORLD_CELL_SIZE * SUBCELL_SIZE;
	}

	return sf::Vector2u(p.x / SUBCELL_SIZE, p.y / SUBCELL_SIZE);
}

sf::Vector2f WorldCell::getVector(const sf::Vector2u& p) const {
	return {1.0f * p.x * SUBCELL_SIZE, 1.0f * p.y * SUBCELL_SIZE};
}

bool WorldCell::rayCast(const sf::Vector2f& start, const sf::Vector2f& end, sf::Vector2f& result) {
	float t = 0.0f;
	const auto dx = end.x - start.x;
	const auto dy = end.y - start.y;
	const auto r = std::sqrt(dx * dx + dy * dy);
	float dt = std::min(0.99f, SUBCELL_SIZE / r);
	const auto dr = (end - start) * dt;

	auto prev = start;
	auto next = prev;

	auto oidx = getIdx(prev);
	auto idx = oidx;

	while (true) {
		idx = getIdx(next);
		if (idx.x != oidx.x && idx.y != oidx.y) {
			sf::Vector2u midx(std::max(idx.x, oidx.x), std::max(idx.y, oidx.y));

			const auto mvec = getVector(midx);
			const auto p = mod(mvec - prev);
			const auto pp = mod(next - prev);
			const auto rr = length(mvec - prev);

			auto tmp0 = athwart(p);
			auto tmp2 = vec(pp, p);
			auto tmp3 = rr;

			next = mvec - athwart(p) * vec(pp, p) * rr;
			t = length(next - start) / r;
			continue;
		} else if (idx.x != oidx.x || idx.y != oidx.y) {
			if (getCellId(idx) != 0) {
				sf::Vector2f mvec;
				if (idx.x == oidx.x) {
					mvec.y = std::max(oidx.y, idx.y) * SUBCELL_SIZE;
					mvec.x = prev.x + (next.x - prev.x) * ((mvec.y - prev.y) / (next.y - prev.y));
				} else {
					mvec.x = std::max(oidx.x, idx.x) * SUBCELL_SIZE;
					mvec.y = prev.y + (next.y - prev.y) * ((mvec.x - prev.x) / (next.x - prev.x));
				}
				result = mvec;
				return true;
			}
		} else {
			if (getCellId(idx) != 0) {
				result = next;
				return true;
			}
		}
		if (t >= 1.0f) {
			break;
		}

		t += dt;
		prev = next;
		oidx = idx;
		next += dr;
	}

	return false;
}