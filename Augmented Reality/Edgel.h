#pragma once
#include "vector2f.h"
//TODO: Implement vector2f or ideally use OpenGL class.

struct Edgel {
public:
	Edgel(float x, float y);

	Vector2f position;
	Vector2f slope;
};

