#pragma once
#include "Edgel.h"
#include "vector2f.h"
#include <vector>


const float LINE_COINCIDENCE_DISTANCE = 0.75f;


class LineSegment {
	Vector2f slope;

	std::vector<Edgel> supportEdgels;

	float distanceFromPointToLine(Edgel& start, Edgel& end, Edgel& point) const;

public:
	Edgel start, end;
	
	LineSegment() = default;
	LineSegment(Edgel start, Edgel end);

	void clearSupportEdgels();
	void addSupportEdgel(Edgel& edgel);
	size_t numberOfSupportEdgels();
	bool coincide(Edgel& edgel);
	Vector2f getLineSlope();
	Vector2f getOrienation();
};

