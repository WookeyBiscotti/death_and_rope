#include "rope.hpp"

void Rope::init(const Vector2f& w, const Vector2f& p)
{
    _points.clear();
    _minusDestruction.clear();
    _points.emplace_back(w);
    _points.emplace_back(p);

    updateLength();
}

void Rope::updateBreak()
{
    if (_points.size() > 2) {
        auto a = sin(_points[_points.size() - 1], _points[_points.size() - 2], _points[_points.size() - 2], _points[_points.size() - 3]) >
                 0;
        if (a == _minusDestruction.back()) {
            _points.erase(_points.end() - 2);
            _minusDestruction.pop_back();
        }
    }
}

void Rope::changeEnd(const Vector2f& p)
{
    if (_points.size() < 2) {
        return;
    }
    _points.back() = p;

    updateBreak();

    updateLength();
}

void Rope::changeEnd(const Vector2f& p, const Vector2f& w)
{
    if (_points.size() < 2) {
        return;
    }
    _points.back() = p;

    updateBreak();

    _points.insert(_points.end() - 2, w);
    _minusDestruction.push_back(
        sin(_points[_points.size() - 1], _points[_points.size() - 2], _points[_points.size() - 2], _points[_points.size() - 3]) < 0);

    updateLength();
}

void Rope::updateLength()
{
    if (_points.size() < 2) {
        return;
    }
    _length = length(_points[_points.size() - 1] - _points[_points.size() - 2]);
}