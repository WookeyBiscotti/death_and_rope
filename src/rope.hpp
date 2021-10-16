#pragma once

#include "vector2.hpp"
#include <vector>

class Rope
{
public:
    void init(const Vector2f& w, const Vector2f& p);

    void changeEnd(const Vector2f& p, const Vector2f& w);
    void changeEnd(const Vector2f& p);

    void updateLength();

private:
    void updateBreak();

private:
    float _length;
    std::vector<Vector2f> _points;
    std::vector<bool> _minusDestruction;
};