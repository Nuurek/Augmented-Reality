#pragma once
#include "Edgel.h"
#include "vector2f.h"
#include <vector>

class LineSegment {
	Edgel start, end;
	Vector2f slope;

	std::vector<Edgel> supportEdgels;

public:
	LineSegment() = default;
	LineSegment(Edgel start, Edgel end);

	void clearSupportEdgels();
	void addSupportEdgel(Edgel& edgel);
};

