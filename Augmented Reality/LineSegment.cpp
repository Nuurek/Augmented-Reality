#include "LineSegment.h"


LineSegment::LineSegment(Edgel start, Edgel end)
	: start(start), end(end) {
	slope = start.slope;
}

bool LineSegment::coincide(Edgel & edgel) {
	if (!start.isOrientationCompatible(edgel)) {
		return false;
	}

	return distanceFromPointToLine(start, end, edgel) < LINE_COINCIDENCE_DISTANCE;
}

Vector2f LineSegment::getStartEndSlope() {
	return end.position - start.position;
}

Vector2f LineSegment::getOrienation() {
	return Vector2f(-start.slope.y, start.slope.x);
}

Vector2f LineSegment::getIntersection(LineSegment & rhs) const {
	Vector2f intersection;

	float denominator = ((rhs.end.position.y - rhs.start.position.y)*(end.position.x - start.position.x)) -
		((rhs.end.position.x - rhs.start.position.x)*(end.position.y - start.position.y));

	float numerator = ((rhs.end.position.x - rhs.start.position.x)*(start.position.y - rhs.start.position.y)) -
		((rhs.end.position.y - rhs.start.position.y)*(start.position.x - rhs.start.position.x));

	float center = numerator / denominator;

	intersection.x = start.position.x + center * (end.position.x - start.position.x);
	intersection.y = start.position.y + center * (end.position.y - start.position.y);

	return intersection;
}

void LineSegment::swapEndPoints() {
	std::swap(start, end);
}

bool LineSegment::isOrientationCompatible(const LineSegment & rhs) const {
	return slope * rhs.slope > LINE_ORIENTATION_THRESHOLD;
}


float LineSegment::distanceFromPointToLine(Edgel & start, Edgel & end, Edgel & point) const {
	float crossProduct = 0.0f;
	crossProduct += (end.position.x - start.position.x) * (point.position.y - start.position.y);
	crossProduct -= (end.position.y - start.position.y) * (point.position.x - start.position.x);

	float lineLength = Vector2f(end.position.x - start.position.x, end.position.y - start.position.y).get_length();

	return fabs(crossProduct / lineLength);
}