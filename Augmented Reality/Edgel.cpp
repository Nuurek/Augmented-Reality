#include "Edgel.h"

Edgel::Edgel(size_t x, size_t y, Vector2f slope) :
	position(Vector2f(static_cast<float>(x), static_cast<float>(y))),
	slope(slope) {}

bool Edgel::isOrientationCompatible(Edgel & rhs) {
	return slope * rhs.slope > ORIENTATION_THRESHOLD;
}
