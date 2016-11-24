#pragma once
#include "Edgel.h"
#include "vector2f.h"
#include <vector>


const float LINE_COINCIDENCE_DISTANCE = 0.75f;


class LineSegment {


	float distanceFromPointToLine(Edgel& start, Edgel& end, Edgel& point) const;

public:
	Edgel start, end;
	Vector2f slope;
	std::vector<Edgel> supportEdgels;
	
	LineSegment() = default;
	LineSegment(Edgel start, Edgel end);

	bool coincide(Edgel& edgel);
	Vector2f getStartEndSlope();
	Vector2f getOrienation();
	void swapEndPoints();
};

