#pragma once
#include "Edgel.h"
#include "vector2f.h"
#include <vector>


const float LINE_COINCIDENCE_DISTANCE = 0.75f;
const float LINE_ORIENTATION_THRESHOLD = cosf((90.0f - ORIENTATION_DEGREE) * static_cast<float>(M_PI) / 180.0f);

class LineSegment {
	float distanceFromPointToLine(Edgel& start, Edgel& end, Edgel& point) const;

public:
	Edgel start, end;
	Vector2f slope;
	std::vector<Edgel> supportEdgels;
	bool isMerged{ false };
	bool startCorner{ false }, endCorner{ false };

	LineSegment() = default;
	LineSegment(Edgel start, Edgel end);

	inline bool operator==(const LineSegment& rhs) const {
		return start == rhs.start && end == rhs.end && slope == rhs.slope;
	}

	bool coincide(Edgel& edgel);
	Vector2f getStartEndSlope();
	Vector2f getOrienation();
	Vector2f getIntersection(LineSegment& rhs) const;
	void swapEndPoints();
	bool isOrientationCompatible(const LineSegment& rhs) const;
};

