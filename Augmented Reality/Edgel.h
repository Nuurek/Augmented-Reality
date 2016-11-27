#pragma once
#include <cmath>
#define _USE_MATH_DEFINES

#include "vector2f.h"

const float ORIENTATION_DEGREE = 67.0f;
const float ORIENTATION_THRESHOLD = cosf(ORIENTATION_DEGREE * static_cast<float>(M_PI) / 180.0f);

struct Edgel {
public:
	Edgel() = default;
	Edgel(size_t x, size_t y, Vector2f slope);

	Vector2f position;
	Vector2f slope;

	inline bool operator==(Edgel& rhs) const {
		return position == rhs.position && slope == rhs.slope;
	}

	bool isOrientationCompatible(Edgel& rhs);
	bool isTheSamePoint(Edgel& rhs);
	bool operator<(Edgel& rhs);
};

