#pragma once
#include "vector2f.h"
//TODO: Implement vector2f or ideally use OpenGL class.

struct Edgel {
public:
	Edgel(size_t x, size_t y, Vector2f slope);

	Vector2f position;
	Vector2f slope;
};

